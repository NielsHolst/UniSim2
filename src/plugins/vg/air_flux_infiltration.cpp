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
    : AirFluxBase(name, parent)
{
    help("computes air infiltration rate");
    Input(leakage).equals(1).help("Infiltration rate at a wind speed of 4 m/s").unit("m3/m3/h");
    Input(windSpeed).imports("outdoors[windSpeed]").unit("m/s");
}

void AirFluxInfiltration::reset() {
    update();
}

void AirFluxInfiltration::update() {
    value = leakage*windSpeed/4.;
}


} //namespace

