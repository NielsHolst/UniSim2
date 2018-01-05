#include <base/data_frame.h>
#include <base/dialog.h>
#include <base/exception.h>
#include <base/publish.h>
#include "food_web.h"
using namespace base;

namespace boxes {

PUBLISH(FoodWeb)

FoodWeb::FoodWeb(QString name, QObject *parent)
    : Box(name, parent) {
    help("resolves food web acquisitions among FoodWebBox objects");
    Input(attackFile).help("Name of file with attack rates matrix");
    Input(gainFile).help("Name of file with gain rates matrix");
    Input(timeStep).equals(1).help("Time step used to compute search rate");
    Input(showMatrices).equals(false).help("Show attack matrix in console?");
}

void FoodWeb::amend() {
    // Open file
    QString fileNamePath = environment().inputFileNamePath(attackFile);
    DataFrame df(this);
    df.read(fileNamePath, DataFrame::ColumnLabelled);
    _n = df.numCol();
    // Food web boxes
    QStringList boxPaths = df.colNames();
    QVector<int> dfColumns;
    _boxes.clear();
    int index = 0;
    for (QString boxPath : boxPaths.toVector()) {
        QVector<Box*> boxes = findMany<Box>(boxPath);
        for (Box *box : boxes) {
            dfColumns.append(index);
            _boxes.append(box);
        }
        ++index;
    }
    // Create matrices and vectors
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

    // Fill attack matrix
    int aColumn(0);
    for (int dfColumn : dfColumns) {
        for (int row=0; row<_n; ++row)
            _a(row, aColumn) = df.at<double>(row, dfColumn);
        ++aColumn;
    }

    // Fill gain matrix
    _g.fill(1.);
    if (!gainFile.isEmpty()) {
        DataFrame df2(this);
        QString fileNamePath = environment().inputFileNamePath(gainFile);
        df2.read(fileNamePath, DataFrame::ColumnLabelled);
        if (df.colNames() != df2.colNames()) {
            ThrowException("Column names in attack and gain file do not match")
                    .value(df.colNames().join(" ")).value2(df.colNames().join(" ")).context(this);
        }
        // Fill matrix
        int aColumn(0);
        for (int dfColumn : dfColumns) {
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
    setupOutputs();
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

void FoodWeb::setupOutputs() {
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
        // Acquistion by attacker (j) pooled on all resources
        double sum = 0.;
        for (int j = 0; j < _n; ++j)
            sum += _s.at(i,j)*_g.at(i,j)*N(j);
        _acqPooled[i] = (D(i) > 0.) ? D(i)*(1. - exp(-sum/D(i))) : 0.;
//        QString s("[%1] %2");
//        dialog().information(s.arg(i).arg(_acqPooled[i]));
    }
}

void FoodWeb::update_acqApprox() {
//Acquistion by attacker (j) of resource (i), first approximation
    for (int i = 0; i < _n; ++i) {
        double Di = D(i);
        // Acquistion by attacker (j) pooled on all resources
        for (int j = 0; j < _n; ++j) {
            _acqApprox(i,j) = (Di > 0.) ? Di*(1. - exp(-_s.at(i,j)*_g.at(i,j)*N(j)/Di))  : 0.;
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
        double Nj = N(j);
        if (fabs(sum - Nj) < 1e-6 && sum > 0.)
            sum = Nj;
        // Put outcomes
        _outcomes[j].loss = sum;
        _outcomes[j].lossRatio = (Nj>0) ? sum/Nj : 0.;
//        if (sum > Nj) {
//            QString s("loss=%1 > density=%2  -  diff=%3");
//            dialog().information(s.arg(sum).arg(Nj).arg(sum-Nj));
//            dialog().information("_s");
//            dialog().information(_s.toString());
//            dialog().information("_acq");
//            dialog().information(_acq.toString());
//        }
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
        if (fabs(sum - Di) < 1e-6 && Di > 0.)
            sum = Di;
        // Put outcomes
        _outcomes[i].supply = sum;
        _outcomes[i].sdRatio = (Di>0) ? sum/Di : 0.;
//        if (sum > D(i)) {
//            QString s("supply=%1 > demand=%2");
//            dialog().information(s.arg(sum).arg(D(i)));
//            dialog().information("_s");
//            dialog().information(_s.toString());
//            dialog().information("_acq");
//            dialog().information(_acq.toString());
//        }
    }
}

} //namespace
