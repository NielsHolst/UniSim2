/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "air_flux_gravitation.h"

using std::min;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(AirFluxGravitation)

AirFluxGravitation::AirFluxGravitation(QString name, QObject *parent)
    : AirFluxBase(name, parent)
{
    help("computes gravitational air flux from top to bottom compartment");
    Input(screensState).imports("construction/shelter[horizontalScreenState]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(indoorsVolume).imports("geometry[indoorsVolume]");
    Input(groundArea).imports("geometry[groundArea]");
}

void AirFluxGravitation::update() {
    double gapArea = (1. - screensState)*groundArea,
           topRho = rhoAir(outdoorsTemperature),
           bottomRho = rhoAir(indoorsTemperature),
           flowRate = (indoorsTemperature > outdoorsTemperature) ?  // m3/h
                      0.06*g*pow(gapArea, 1.5)*sqrt((topRho - bottomRho)/topRho)*screensState*2 : 0.;

    value = flowRate/indoorsVolume;
}

} //namespace

