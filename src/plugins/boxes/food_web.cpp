/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QSet>
#include <QStringList>
#include <QTextStream>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "food_web.h"

using namespace base;
using namespace TestNum;

namespace boxes {

PUBLISH(FoodWeb)

FoodWeb::FoodWeb(QString name, QObject *parent)
    : Box(name, parent), _foodWeb(this), _a(this) {
    help("resolves food web acquisitions among Predator and Parasite objects");
    Input(timeStep).equals(1.).help("Simulation time step");
}

void FoodWeb::amend() {
    collectFoodWeb();
    collectInputs();
    createOutputs();
}

void FoodWeb::reset() {
    _s.fill(0.);
    _S.fill(0.);
    _Stotal.fill(0.);
    _sdRatio.fill(0.);
    _Xloss.fill(0.);
    _XlossTotal.fill(0.);
    _mortality.fill(0.);
}

void FoodWeb::update() {
    computeOutputs();
    pushOutputs();
}

inline void info(QString s, int i, double x) {
    QString msg = s + +"[" + QString::number(i) +"] = " + QString::number(x);
//    dialog().information(msg);
}

void FoodWeb::computeOutputs() {
    _s.fill(0.);
    // (1) For every resource i, compute the search rates s_ij for all it attackers j by by eq. 8.
    for (int i=0; i<_nPrey; ++i) {
        double sum(0);
        for (int j=0; j<_nPredators; ++j)
            sum += Y(j)*a(i,j);
        double s_pooled = 1. - exp(-sum*timeStep);
        info("s_pooled", i, s_pooled);

        QVector<double> s_specific(_nPredators);
        sum = 0.;
        for (int j=0; j<_nPredators; ++j)
            sum += s_specific[j] = 1. - exp(-Y(j)*a(i,j));
        for (int j=0; j<_nPredators; ++j) {
            _s(i,j) = sum==0. ? 0. : s_specific.at(j)/sum*s_pooled;
            info("_s", j, _s(i,j));
        }
    }
    // (2) For every attacker j, compute the supply obtained Î”S_ij from each of its resources i by eq. 12.
    _Stotal.fill(0.);
    _sdRatio.fill(0.);
    for (int j=0; j<_nPredators; ++j) {
        double D = FoodWeb::D(j);
        if (D>0.) {
            double sum(0), g_sum(0);
            for (int i=0; i<_nPrey; ++i) {
                sum += g(i,j)*s(i,j)*X(i);
            }
            double S_total = D*(1. - exp(-sum/D));
            info("S_total", j, S_total);

            QVector<double> S_specific(_nPrey);
            sum = 0.;
            for (int i=0; i<_nPrey; ++i) {
                sum += S_specific[i] = D*(1. - exp(-g(i,j)*s(i,j)*X(i)/D));
                g_sum += g(i,j);
            }
            for (int i=0; i<_nPrey; ++i) {
//                _Stotal[j] += _S(i,j) = S_specific.at(i)/sum*g(i,j)/g_sum*S_total;
                _Stotal[j] += _S(i,j) = sum==0. ? 0. : S_specific.at(i)/sum*S_total;
                info("_S", j, _S(i,j));
            }
        }
        else {
            for (int i=0; i<_nPrey; ++i) {
                _S(i,j) = 0.;
            }
        }
    }
    // Updat supply/demand ratios
    for (int j=0; j<_nPredators; ++j) {
        double D = FoodWeb::D(j);
        _sdRatio[j] = D==0. ? 0. : _Stotal.at(j)/D;
    }
    //(3) For every attacker j, compute the loss incured -Î”X_ij on each of its resources i by eq. 13.
    _XlossTotal.fill(0);
    _mortality.fill(0.);
    for (int j=0; j<_nPredators; ++j) {
        info("Predator", j, 0.);
        for (int i=0; i<_nPrey; ++i) {
            _XlossTotal[i] += _Xloss(i,j) = g(i,j)==0. ? 0. : S(i,j)/g(i,j);
            info("_Xloss", i, _Xloss(i,j));
        }
    }
    for (int i=0; i<_nPrey; ++i) {
        // Use snapTo for numerical inaccuracy at small numbers
        snapTo(_XlossTotal[i], X(i));
        double mortality = (X(i)==0.) ? 0. :_XlossTotal[i]/X(i);
        snapTo(mortality, .1);
        _mortality[i] =  mortality;
        info("_XlossTotal", i, _XlossTotal[i]);
        info("_mortality", i, _mortality[i]);
     }
}

void FoodWeb::pushOutputs() {
    for (Box *box : _descendents)
        box->updateImports();
}

inline QStringList setToList(QSet<QString> set) {
    QStringList list = QStringList(set.values());
    list.sort();
    return list;
}

void FoodWeb::collectFoodWeb() {
    QVector<Box*> predators = findMany<Box>("./*<Predator>"),
                  prey = findMany<Box>("./*<Predator>/*<Prey>");
    QSet<QString> predatorNamesUnique, preyNamesUnique;
    for (Box *box : predators)
        predatorNamesUnique << box->name();
    for (Box *box : prey)
        preyNamesUnique << box->name();

    _predatorNames = setToList(predatorNamesUnique);
    _preyNames = setToList(preyNamesUnique);
    _nPredators = _predatorNames.size();
    _nPrey = _preyNames.size();

    _foodWeb.resize(_nPrey, _nPredators);
    for (int j=0; j<_nPredators; ++j) {
        for (int i=0; i<_nPrey; ++i) {
            QString path = "./" + _predatorNames.at(j) + "<Predator>/"
                                + _preyNames.at(i) + "<Prey>";
            _foodWeb(i,j) = findMaybeOne<Box>(path);
        }
    }

    _descendents = predators;
    _descendents.append(prey);
}

void FoodWeb::collectInputs() {
    // Allocate input buffers
    _a.resize(_nPrey, _nPredators);
    _g.resize(_nPrey, _nPredators);
    _X.resize(_nPrey);
    _Y.resize(_nPredators);
    _D.resize(_nPredators);

    // Collect a and g matrices
    for (int i=0; i<_nPrey; ++i)
        for (int j=0; j<_nPredators; ++j) {
            Box *box = _foodWeb.at(i,j);
            _a(i,j) = box ? box->port("attackRate")->valuePtr<double>() : nullptr;
            _g(i,j) = box ? box->port("gainFactor")->valuePtr<double>() : nullptr;
        }
    // Collect prey vector
    for (int i=0; i<_nPrey; ++i) {
        QVector<Box*> prey = findMany<Box>("./*<Predator>/" + _preyNames.at(i) + "<Prey>");
        QSet<QString> densityPaths;
        // Check that prey density paths are identical
        for (Box *anotherPrey : prey) {
            QString path = anotherPrey->port("density")->importPath();
            if (!path.isEmpty())
                densityPaths << path;
        }
        if (densityPaths.size() > 1) {
            ThrowException("Prey with same name must refer to the same source density").
                    value(_preyNames.at(i)).
                    value2(QStringList(densityPaths.values()).join(",")).
                    context(this);
        }
        Q_ASSERT(!prey.isEmpty());
        _X[i] = prey[0]->port("density")->valuePtr<double>();
    }
    // Collect predator and demand vectors
    for (int j=0; j<_nPredators; ++j) {
        QVector<Box*> predators = findMany<Box>("./" + _predatorNames.at(j) + "<Predator>");
        if (predators.size() > 1) {
            ThrowException("Predators must have unique names with FoodWeb").
                    value(_predatorNames.at(j)).
                    value2(predators.size()).
                    context(this);
        }
        Q_ASSERT(!predators.isEmpty());
        _Y[j] = predators[0]->port("density")->valuePtr<double>();
        _D[j] = predators[0]->port("demand")->valuePtr<double>();
    }
}

void FoodWeb::createOutputs() {
    // Allocate output buffers
    _s.resize(_nPrey, _nPredators);
    _S.resize(_nPrey, _nPredators);
    _Xloss.resize(_nPrey, _nPredators);
    _XlossTotal.resize(_nPrey);
    _mortality.resize(_nPrey);
    _Stotal.resize(_nPredators);
    _sdRatio.resize(_nPredators);

    // Create predator-prey output ports
    for (int i=0; i<_nPrey; ++i) {
        for (int j=0; j<_nPredators; ++j) {
            QString suffix = "_" + _predatorNames.at(j) + "_" + _preyNames.at(i);
            NamedOutput("s" + suffix, _s(i,j));
            NamedOutput("supply" + suffix, _S(i,j));
            NamedOutput("loss" + suffix, _Xloss(i,j));
        }
    }

    // Create predator output ports
    for (int j=0; j<_nPredators; ++j) {
        QString suffix = "_" + _predatorNames.at(j);
        NamedOutput("supply" + suffix, _Stotal[j]);
        NamedOutput("sdRatio" + suffix, _sdRatio[j]);    }

    // Create prey output ports
    for (int i=0; i<_nPrey; ++i) {
        QString suffix = "_" + _preyNames.at(i);
        NamedOutput("loss" + suffix, _XlossTotal[i]);
        NamedOutput("mortality" + suffix, _mortality[i]);
    }
}

inline QString aph(QString s) {
    return "\"" + s + "\"";
}

QString FoodWeb::showMatrix(base::Matrix2D<const double*> m) {
    QStringList rows, rowNames;
    for (int j=0; j<_nPredators; ++j) {
        QStringList row;
        for (int i=0; i<_nPrey; ++i) {
            double value = m.at(i,j) ? *m.at(i,j) : 0.;
            row << QString::number(value);
        }
        QString s = _predatorNames.at(j) + " = c(" + row.join(",") + ")";
        rows << s;
    }
    for (int i=0; i<_nPrey; ++i)
        rowNames << aph(_preyNames.at(i));

    QString s = QString() +
        "M = data.frame (\n" +
        rows.join(",\n") +
        "\n)\n" +
        "rownames(M) = " +
        "c(" + rowNames.join(",") + ")\n" +
        "M\n";
    return s;
}

QString FoodWeb::showDensities() {
    QStringList line;
    for (int j=0; j<_nPredators; ++j) {
        line << (_predatorNames.at(j) + " = " + QString::number(*_Y.at(j)));
    }
    QString y = QString() +
        "Y = data.frame (\n" +
        line.join(",\n") +
        "\n)\n" +
        "Y\n";

    line.clear();
    for (int i=0; i<_nPrey; ++i) {
        line << QString::number(*_X.at(i));
    }
    QString x = line.join("\n");


    return y+x;
}

} //namespace

