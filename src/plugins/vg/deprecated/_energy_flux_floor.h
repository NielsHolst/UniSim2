/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ENERGY_FLUX_FLOOR_H
#define ENERGY_FLUX_FLOOR_H

#include "energy_flux_base.h"

namespace vg {

class EnergyFluxFloor : public EnergyFluxBase
{
public:
    EnergyFluxFloor(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double Uindoors, Usoil, heatCapacity, emissivity,
        indoorsTemperature, soilTemperature,
        height, timeStep;
    // Outputs
    double temperature;
};
} //namespace


#endif
