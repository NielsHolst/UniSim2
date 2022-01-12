/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/environment.h>
#include <base/random_generator.h>
#include <base/random_order.h>
#include "output_r.h"
#include "randomiser_base.h"
#include "random_base.h"

#include <base/dialog.h>

using namespace base;

namespace boxes {

RandomiserBase::RandomiserBase(QString name, Box *parent)
    : Box(name, parent), matricesFilled(false)
{
    setClassName("RandomiserBase");
    Input(iteration).imports("/*[iteration]");
    Input(iterations).imports("/*[iterations]");
    Input(useFixed).equals(false).help("Used fixed value instead of random values?");
    Input(doSensitivityAnalysis).equals(false).help("Carry out a sensitivity analysis?");
    Input(bootstrapSize).equals(1000).help("Size of bootstrap sample (cheap in computation time); only used in sensitivity analysis");
    Input(seed).help("Seed for random numbers; if this is zero a random seed value will be used");
    Input(drawAtInitialize).equals(false).help("Draw a value when a box is initialized?");
    Input(drawAtReset).equals(true).help("Draw a value when a box is reset?");
    Input(drawAtUpdate).equals(false).help("Draw a value when a box is updated?");
    Output(numVariables).noReset().help("Number of variables which are randomised").unit("[0;inf)");
}

void RandomiserBase::initialize() {
    // Set random seed in global generator
    int theSeed = (seed==0) ? static_cast<int>(std::time(nullptr)) : seed;
    base::seedRandomGenerator(theSeed);
    // Initialize me
    findRandomVariables();
    inializeGenerator();
    checkIterations(iterations);
    // Set up SA
    matricesFilled = false;
    if (doSensitivityAnalysis) {
        setBaseSampleSize();
    }
}

namespace {
    void copyColumn(const Matrix<double> &a, Matrix<double> &b, int col) {
        for (int row=0; row<a.numRow(); ++row)
            b(row,col) = a.at(row,col);
    }
}

RandomBase* RandomiserBase::variableB(RandomBase *A) {
    RandomBase *variableB = A->findMaybeOne<RandomBase*>("./B");
    return variableB ? variableB : A;
}

void RandomiserBase::fillMatrices() {
    int &k(numVariables);
    A.resize(N, k);
    B.resize(N, k);
    C.resize(N, k);
    for (RandomBase *variable : randomVariables) {
        variable->port("drawAtReset")->equals(true);
        variable->port("useFixed")->equals(false);
    }
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < k; ++col) {
            RandomBase *variable = randomVariables.at(col),
                       *variableB_ = variableB(variable);
            variable->resetFamily();
            A(row, col) = variable->port("value")->value<double>();
            variableB_->resetFamily();
            B(row, col) =
            C(row,col) = variableB_->port("value")->value<double>();
        }
    }
//    RandomOrder shuffle(2*N, nullptr);
//    for (int row = 0; row < N; ++row) {
//        for (int col = 0; col<k; ++col) {
//            A(row, col) = AB.at(shuffle.at(row), col);
//            B(row, col) = C(row, col) = AB.at(shuffle.at(row+N), col);
//        }
//    }
    for (RandomBase *variable : randomVariables) {
        variable->port("useFixed")->equals(true);
        variableB(variable)->port("useFixed")->equals(true);
    }
    matricesFilled = true;
}

void RandomiserBase::reset() {
    int &k(numVariables);
    if (!doSensitivityAnalysis) return;
    if (!matricesFilled) fillMatrices();
    phase = (iteration <= N) ? 'A' :
            (iteration <= 2*N) ? 'B' : 'C';
    int row = (iteration-1) % N,
        col = 0;
    switch (phase) {
    case 'A':
        for (RandomBase *variable : randomVariables) {
             variable->port("fixed")->equals(A.at(row, col++));
        }
        break;
    case 'B':
        for (RandomBase *variable : randomVariables) {
             variable->port("fixed")->equals(B.at(row, col++));
        }
        break;
    case 'C':
        // Note that iteration counts from 1 not 0
        if (row==0) {
            int colSwap = (iteration - 1)/N - 2;
            Q_ASSERT(colSwap>=0 && colSwap < k);
            if (colSwap > 0)
                copyColumn(B,C,colSwap-1);
            copyColumn(A,C,colSwap);
        }
        for (RandomBase *variable : randomVariables) {
             variable->port("fixed")->equals(C.at(row, col++));
        }
    }
}

void RandomiserBase::findRandomVariables(){
    randomVariables.clear();
    QVector<RandomBase*> candidates = findMany<RandomBase*>("../*<RandomBase>");
    for (RandomBase *candidate : candidates) {
        if (!candidate->port("useFixed")->value<bool>())
            randomVariables << candidate;
    }
    numVariables = randomVariables.size();
}

void RandomiserBase::setBaseSampleSize() {
    int &k(numVariables);
    N = iterations/(k+2);
    if (iterations%(k+2) > 0)
        ThrowException("Unexpected: Illegal number of iterations").value(iterations).context(this);
}

void RandomiserBase::checkIterationsSimple(int iterations) {
    int N = static_cast<int>(floor(iterations/double(2+numVariables)));
    if (N<4)
        N = 4;
    int N1 = N*(2+numVariables),
        N2 = (N+1)*(2+numVariables);
    QString msg("Simulation iterations must equal (2+k)*N (k=%1) to run a sensitivity analysis"),
            hint("Use instead, for example, %1 or %2 iterations");
    if (iterations!=N1 && iterations!=N2) {
        ThrowException(msg.arg(numVariables)).
                value(iterations).
                hint(hint.arg(N1).arg(N2)).
                context(this);
    }
}

void RandomiserBase::debrief() {
    OutputR *outputR = findMaybeOne<OutputR*>("*");
    if (outputR &&doSensitivityAnalysis) {
        outputR->addRCode("sobol_k = " + QString::number(numVariables) +
                          "; sobol_N = " + QString::number(N) +
                          "; sobol_B = " + QString::number(bootstrapSize) +"\n" +
                          "source(\"" + environment().inputFileNamePath("scripts/begin-sobol.R") + "\")"
                          );
    }
}

}
