/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FOOD_WEB
#define FOOD_WEB
#include <base/box.h>
#include <base/data_frame.h>
#include <base/matrix.h>

namespace boxes {

class FoodWeb : public base::Box
{
public: 
    FoodWeb(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    QString attackFileFirst, attackFile, gainFile;
    bool showMatrices;
    double timeStep;
    // Data
    QString _currentAttackFile;
    base::DataFrame _attackDf;
    QVector<int> _dfColumns;
    int _n;
    QVector<base::Box*> _boxes;
    QVector<const double *> _densities, _demands;
    base::Matrix<double> _a, _g, _sApprox, _s, _acqApprox, _acq;
    QVector<double> _sPooled, _acqPooled;
    bool _outputsCreated;
    struct Outcome {
        double supply, loss,
            sdRatio, lossRatio;
    };
    QVector<Outcome> _outcomes;

    // Methods
    void readAttackDataFrame();
    void fillAttackMatrix();
    void createOutputs();
    void resetOutcomes();
    double N(int i) const { return *_densities.at(i); }
    double D(int i) const { return *_demands.at(i); }
    void update_sPooled();
    void update_sApprox();
    void update_s();
    void update_acqPooled();
    void update_acqApprox();
    void update_acq();
    void update_losses();
    void update_supplies();
};

} //namespace
#endif
