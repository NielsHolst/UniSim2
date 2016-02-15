/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BUDGET_H
#define VG_BUDGET_H

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
