/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_AIR_FLUX_INFILTRATION_H
#define VG_AIR_FLUX_INFILTRATION_H

#include <base/box.h>

namespace vg {

class AirFluxInfiltration : public base::Box
{
public:
    AirFluxInfiltration(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double leakage, windSpeed, airTransmissivity;

    // Variables
    double value;

};
} //namespace


#endif
