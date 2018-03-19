/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AIR_FLUX_GRAVITATION_H
#define AIR_FLUX_GRAVITATION_H

#include <base/box.h>

namespace vg {

class AirFluxGravitation : public base::Box
{
public:
    AirFluxGravitation(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double screensState, outdoorsTemperature, indoorsTemperature, indoorsVolume, groundArea;

    // Outputs
    double value;

    // Methods
    double maxFiniteRate(double finiteRate1) const;
};
} //namespace


#endif
