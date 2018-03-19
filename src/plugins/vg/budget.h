/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BUDGET_H
#define BUDGET_H

#include <base/box.h>

namespace vg {

class Budget : public base::Box
{
public:
    Budget(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double heatingEnergyFlux, growthLightsEnergyFlux, co2Flux, dt;
    QString energyUnit;
    // Outputs
    double heatingEnergyTotal, growthLightsEnergyTotal, co2Total;
    // Data
    double eUnit;

};
} //namespace


#endif
