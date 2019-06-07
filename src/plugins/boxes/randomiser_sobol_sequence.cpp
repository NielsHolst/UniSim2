#include <base/exception.h>
#include <base/publish.h>
#include <base/random_generator.h>
#include "randomiser_sobol_sequence.h"
#include "random_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomiserSobolSequence)

RandomiserSobolSequence::RandomiserSobolSequence(QString name, QObject *parent)
    : RandomiserBase(name, parent), sobol(nullptr), variate(nullptr)
{
    help("draws quasi-random numbers [0;1) from the Sobol' sequence");
}

RandomiserSobolSequence::~RandomiserSobolSequence() {
    delete sobol;
    delete variate;
    sobol = nullptr;
    variate = nullptr;
}

void RandomiserSobolSequence::inializeGenerator() {
    delete sobol;
    delete variate;
    sobol = new boost::random::sobol(static_cast<unsigned int>(2*numVariables));
    variate = new Variate(*sobol, distribution);
}

void RandomiserSobolSequence::checkIterations(int iterations) {
    int &k(numVariables);
    if (doSensitivityAnalysis) {
        int N = static_cast<int>(floor(log2(iterations/double(2+k))));
        if (N<2)
            N = 2;
        int N1 = (2<<(N-1))*(2+k),
            N2 = (2<<N)*(2+k);
        QString msg("Simulation iterations must equal (2+k)*N (k=%1, N=2^i) to run a sensitivity analysis with a Sobol' sequence for random numbers"),
                hint("Use instead, for example, %1 or %2 iterations");
        if (iterations!=N1 && iterations!=N2) {
            ThrowException(msg.arg(k)).
                    value(iterations).
                    hint(hint.arg(N1).arg(N2)).
                    context(this);
        }
    }
    else {
        int n = static_cast<int>(floor(log2(iterations)));
        if (n<32)
            n = 32;
        int N1 = 2<<(n-1),
            N2 = 2<<n;
        QString hint("Use instead, for example, %1 or %2 iterations %3");
        if (iterations!=N1 && iterations!=N2) {
            ThrowException("Simulation iterations must be a power of 2 when a Sobol' sequence is used for random numbers").
                    value(iterations).
                    hint(hint.arg(N1).arg(N2)).
                    context(this);
        }
    }
}

int RandomiserSobolSequence::numStrata() const {
    return 1;
}

double RandomiserSobolSequence::draw01() {
    return (*variate)();
}

}
