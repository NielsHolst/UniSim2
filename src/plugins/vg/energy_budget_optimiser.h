/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ENERGY_BUDGET_OPTIMISER_H
#define ENERGY_BUDGET_OPTIMISER_H
#include <QVector>
#include <base/box.h>

namespace vg {

class EnergyBudgetOptimiser : public base::Box
{
public: 
    EnergyBudgetOptimiser(QString name, QObject *parent);
    // standard methods
    void initialize();
    void update();
private:
    QVector<QString> inputs;
    double value;
    QVector<base::Port*> ports;
};

} //namespace
#endif
