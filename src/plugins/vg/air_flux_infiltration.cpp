/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    Input(leakage).equals(1).help("The infiltration rate [m3/m3/h] at a wind speed of 4 m/s");
    Input(windSpeed).imports("outdoors[windSpeed]");
    Output(value).help("Relative rate of air exchange [m3/m3/h]");
}

void AirFluxInfiltration::reset() {
    update();
}

void AirFluxInfiltration::update() {
    value = leakage*windSpeed/4.;
}


} //namespace

