/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
