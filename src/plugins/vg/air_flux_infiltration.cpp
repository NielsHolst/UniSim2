/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "air_flux_infiltration.h"

using namespace base;

namespace vg {

PUBLISH(AirFluxInfiltration)

AirFluxInfiltration::AirFluxInfiltration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes air infiltration rate");
    Input(leakage).equals(1).help("The infiltration rate (m3/m3/h) at a wind speed of 4 m/s");
    Input(windSpeed).imports("outdoors[windSpeed]");
    Output(value);
}

void AirFluxInfiltration::reset() {
    update();
}

void AirFluxInfiltration::update() {
    value = leakage*windSpeed/4.;
}


} //namespace

