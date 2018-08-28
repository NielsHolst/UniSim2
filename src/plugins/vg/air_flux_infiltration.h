/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AIR_FLUX_INFILTRATION_H
#define AIR_FLUX_INFILTRATION_H

#include "air_flux_base.h"

namespace vg {

class AirFluxInfiltration : public AirFluxBase
{
public:
    AirFluxInfiltration(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double leakage, windSpeed;
};
} //namespace


#endif
