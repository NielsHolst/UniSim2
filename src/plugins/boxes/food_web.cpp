/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "food_web.h"
using namespace base;

namespace boxes {

PUBLISH(FoodWeb)

FoodWeb::FoodWeb(QString name, QObject *parent)
    : Box(name, parent), _attackDf(this), _outputsCreated(false) {
    help("resolves food web acquisitions among FoodWebBox objects");
    Input(attackFile).help("Name of file with attack rates matrix");
    Input(gainFile).help("Name of file with gain rates matrix");
    Input(attackFileFirst).help("Only needed if attackFile is changed with every iteration; will then be used for first iteration");
    Input(timeStep).equals(1).help("Time step used to compute search rate");
    Input(showMatrices).equals(false).help("Show attack matrix in console?");
}

void FoodWeb::amend() {
    ExceptionContext(this);
    // Read attack file
    readAttackDataFrame();
    _n = _attackDf.numCol();

    // Food web boxes
    QStringList boxPaths = _attackDf.colNames();
    _dfColumns.clear();
    _boxes.clear();
    int index = 0;
    for (QString boxPath : boxPaths.toVector()) {
        QVector<Box*> boxes = findMany<Box>(boxPath);
        for (Box *box : boxes) {
            _dfColumns.append(index);
            _boxes.append(box);
        }
        ++index;
    }
    // Resize matrices and vectors
    _a.resize(_n,_n);
    _g.resize(_n,_n);
    _sApprox.resize(_n,_n);
    _s.resize(_n,_n);
    _acqApprox.resize(_n,_n);
    _acq.resize(_n,_n);
    _densities.resize(_n);
    _demands.resize(_n);
    _sPooled.resize(_n);
    _acqPooled.resize(_n);
    _outcomes.resize(_n);

    fillAttackMatrix();

    // Fill gain matrix
    _g.fill(1.);
    if (!gainFile.isEmpty()) {
        DataFrame df2(this);
        QString fileNamePath = environment().inputFileNamePath(gainFile);
        df2.read(fileNamePath, DataFrame::ColumnLabelled);
        if (_attackDf.colNames() != df2.colNames()) {
            ThrowException("Column names in attack and gain file do not match")
                    .value(_attackDf.colNames().join(" ")).value2(df2.colNames().join(" ")).context(this);
        }
        // Fill matrix
        int aColumn(0);
        for (int dfColumn : _dfColumns) {
            for (int row=0; row<_n; ++row)
                _g(row, aColumn) = df2.at<double>(row, dfColumn);
            ++aColumn;
        }
    }

    // Fetch pointers to output ports
    for (int i=0; i<_n; ++i) {
        _densities[i] = _boxes.at(i)->port("density")->valuePtr<double>();
        _demands[i] = _boxes.at(i)->port("demand")->valuePtr<double>();
    }
    // Create output ports
    createOutputs();
    // Show
    if (showMatrices) {
        QString s =
                "Attack matrix:\n" +
                _a.toString() +
                "Gain matrix:\n" +
                _g.toString() +
                "Food web boxes:\n";
        for (Box *box: _boxes)
            s += box->fullName() + "\n";
        dialog().information(s);
    }
}

void FoodWeb::readAttackDataFrame() {
    QString fileName = attackFile.isEmpty() ? attackFileFirst : attackFile;
    QString fileNamePath = environment().inputFileNamePath(fileName);
    _attackDf.read(fileNamePath, DataFrame::ColumnLabelled);
}

void FoodWeb::fillAttackMatrix() {
    int aColumn(0);
    for (int dfColumn : _dfColumns) {
        for (int row=0; row<_n; ++row)
            _a(row, aColumn) = _attackDf.at<double>(row, dfColumn);
        ++aColumn;
    }
}

void FoodWeb::createOutputs() {
    if (_outputsCreated) return;
    _outputsCreated = true;
    for (int i = 0; i < _n; ++i) {
        QString lossName   = QString("loss_%1").arg(i),
                supplyName = QString("supply_%1").arg(i),
                sdRatioName = QString("sdRatio_%1").arg(i),
                lossRatioName = QString("lossRatio_%1").arg(i),
                importLossName   = QString("%1[%2]").arg(fullName()).arg(lossName),
                importSupplyName = QString("%1[%2]").arg(fullName()).arg(supplyName),
                importSdRatioName = QString("%1[%2]").arg(fullName()).arg(sdRatioName),
                importLossRatioName = QString("%1[%2]").arg(fullName()).arg(lossRatioName);

        NamedOutput(lossName,   _outcomes[i].loss);
        NamedOutput(supplyName, _outcomes[i].supply);
        NamedOutput(sdRatioName, _outcomes[i].sdRatio);
        NamedOutput(lossRatioName, _outcomes[i].lossRatio);
        _boxes[i]->port("loss")  ->imports(importLossName);
        _boxes[i]->port("supply")->imports(importSupplyName);
        _boxes[i]->port("supplyDemandRatio")->imports(importSdRatioName);
        _boxes[i]->port("lossRatio")->imports(importLossRatioName);
//        QString s = "[%1] %2 => %3";
//        dialog().information(s.arg(i).arg(lossName).arg(_boxes[i]->fullName()));
//        dialog().information(s.arg(i).arg(supplyName).arg(_boxes[i]->fullName()));
    }
}

void FoodWeb::resetOutcomes() {
    for (Outcome outcome : _outcomes) {
        outcome = Outcome{0,0,0,0};
    }
}

void FoodWeb::reset() {
    ExceptionContext(this);
    if (attackFile != _currentAttackFile) {
        readAttackDataFrame();
        int n = _attackDf.numCol();
        if (n!=_n)
            ThrowException("Attack files must have the same number of columns")
                    .value(attackFile).value2(_currentAttackFile).context(this);
        fillAttackMatrix();
        _currentAttackFile = attackFile;
    }
    resetOutcomes();
}

void FoodWeb::update() {
    update_sPooled();
    update_sApprox();
    update_s();
    update_acqPooled();
    update_acqApprox();
    update_acq();
    update_losses();
    update_supplies();
}

void FoodWeb::update_sPooled() {
    // Search rate for resource (j) pooled on all attackers
    for (int j = 0; j < _n; ++j) {
        double sum = 0.;
        for (int i = 0; i < _n; ++i)
            sum += _a.at(i,j)*N(i)*timeStep;
        _sPooled[j] = 1. - exp(-sum);
    }
}

void FoodWeb::update_sApprox() {
    // Search rate for resource (j) by attacker (i) , first approximation
    for (int j = 0; j < _n; ++j) {
        for (int i = 0; i < _n; ++i)
            _sApprox(i,j) = 1. - exp(-_a.at(i,j)*N(i));
    }
}

void FoodWeb::update_s() {
    // Search rate for resource (j) by attacker (i), final
    for (int j = 0; j < _n; ++j) {
        double sum_sApprox = 0;
        for (int i = 0; i < _n; ++i)
            sum_sApprox += _sApprox.at(i,j);
        for (int i = 0; i < _n; ++i)
            _s(i,j) = (sum_sApprox > 0) ? _sApprox.at(i,j)/sum_sApprox*_sPooled.at(j) : 0.;
    }
}

void FoodWeb::update_acqPooled() {
//    dialog().information("_s");
    for (int i = 0; i < _n; ++i) {
        double Di = D(i);
        // Acquistion by attacker (i) pooled on all resources
        double sum = 0.;
        for (int j = 0; j < _n; ++j)
            sum += _s.at(i,j)*_g.at(i,j)*N(j);
        double acq = (D(i) > 0.) ? D(i)*(1. - exp(-sum/Di)) : 0.;
        // Round off error
        if (acq > Di) acq = Di;
        if (acq > sum) acq = sum;
        _acqPooled[i] = acq;
//        QString s("[%1] %2");
//        dialog().information(s.arg(i).arg(_acqPooled[i]));
    }
}

void FoodWeb::update_acqApprox() {
//Acquistion by attacker (j) of resource (i), first approximation
    for (int i = 0; i < _n; ++i) {
        double Di = D(i);
//        dialog().information("Di = " + QString::number(Di));
        // Acquistion by attacker (j) pooled on all resources
        for (int j = 0; j < _n; ++j) {
            double Nj = N(j);
//            dialog().information("  Nj = " + QString::number(N(j)));
            double acq = (Di > 0.) ? Di*(1. - exp(-_s.at(i,j)*_g.at(i,j)*Nj/Di))  : 0.;
            // Round off error
            if (acq > Di) acq = Di;
            if (acq > Nj) acq = Nj;
            _acqApprox(i,j) = acq;
        }
    }
//    dialog().information("_acqApprox");
//    dialog().information(_acqApprox.toString());
}

void FoodWeb::update_acq() {
    for (int i = 0; i < _n; ++i) {
        double sum_acqApprox = 0;
        for (int j = 0; j < _n; ++j)
            sum_acqApprox += _acqApprox.at(i,j);
        for (int j = 0; j < _n; ++j) {
            _acq(i,j) = (sum_acqApprox > 0) ? _acqApprox.at(i,j)/sum_acqApprox*_acqPooled.at(i) : 0.;
//            QString s("[%1 %2] %3");
//            dialog().information(s.arg(i).arg(j).arg(_acq(i,j)));
        }
    }
//    dialog().information("_acq");
//    dialog().information(_acq.toString());
}

void FoodWeb::update_losses() {
    for (int j=0; j<_n; ++j) {
        double sum = 0.;
        for (int i=0; i<_n; ++i) {
            double g = _g.at(i,j);
            sum += (g > 0.) ? _acq.at(i,j)/g : 0.;
        }
        // Round off error
        double Nj = N(j),
               lossRatio = (Nj>0) ? sum/Nj : 0.;
        TestNum::snapTo(lossRatio, 1.);

        // HACK!
        if (lossRatio > 1.) lossRatio = 1.;

        double loss = TestNum::eq(lossRatio, 1.) ? Nj : sum;

        // Put outcomes
        _outcomes[j].loss = loss;
        _outcomes[j].lossRatio = lossRatio;
        if (lossRatio >1. || loss > Nj) {
            QString s("loss=%1 > density=%2  -  diff=%3 - lossRatio=%4");
            dialog().information(s.arg(loss).arg(Nj).arg(loss-Nj).arg(lossRatio));
            dialog().information("_s");
            dialog().information(_s.toString());
            dialog().information("_acq");
            dialog().information(_acq.toString());
        }
    }
}

void FoodWeb::update_supplies() {
    for (int i=0; i<_n; ++i) {
        double sum = 0.;
        for (int j=0; j<_n; ++j) {
            sum += _acq.at(i,j);
        }
        // Round off error
        double Di = D(i);
        TestNum::snapTo(sum, Di);
        // Put outcomes
        _outcomes[i].supply = sum;
        _outcomes[i].sdRatio = (Di>0) ? sum/Di : 0.;
        if (sum > D(i)) {
            QString s("supply=%1 > demand=%2");
            dialog().information(s.arg(sum).arg(D(i)));
            dialog().information("_s");
            dialog().information(_s.toString());
            dialog().information("_acq");
            dialog().information(_acq.toString());
        }
    }
}

} //namespace

