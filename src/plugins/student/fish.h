/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FISH_H
#define FISH_H
#include <QVector>
#include <base/box.h>
#include <base/port.h>

namespace student {

class Fish : public base::Box
{
public:
    Fish(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    int initialNumber;
    double initialWeight, timeStepDays;
    bool death;
    int dayOfYear;
    // Outputs
    int number;
    double totalWeight;
    // Methods
    // virtual double conversionEfficiency(double weight) = 0;   in base class (pure virtual function)
    double conversionEfficiency(double weight);
    double growthRate(double weight);
    void effectuateMortality();
    // Data
    QVector<double> individuals;
    int currentDayOfYear;
};

}

#endif
