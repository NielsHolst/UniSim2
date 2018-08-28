/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AIR_FLUX_GRAVITATION_H
#define AIR_FLUX_GRAVITATION_H

#include "air_flux_base.h"

namespace vg {

class AirFluxGravitation : public AirFluxBase
{
public:
    AirFluxGravitation(QString name, QObject *parent);
    void update();

private:
    // Inputs
    double screensState, outdoorsTemperature, indoorsTemperature, indoorsVolume, groundArea;

    // Methods
    double maxFiniteRate(double finiteRate1) const;
};
} //namespace


#endif
