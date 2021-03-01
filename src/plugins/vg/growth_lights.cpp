/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "growth_lights.h"

using namespace base;

namespace vg {

PUBLISH(GrowthLights)

GrowthLights::GrowthLights(QString name, QObject *parent)
    : HeatTransferLayerBase(name, parent)
{
    help("sums power use and radiation from growth light");
    port("parFluxDown")->imports("./*[parFluxDown]").transform(Sum);
    port("swFluxDown")->imports("./*[swFluxDown]").transform(Sum);
    port("lwFluxDown")->imports("./*[lwFluxDown]").transform(Sum);
    port("area")->imports("construction/geometry[groundArea]",CA);
    Input(lightsCurrentlyOn).imports("./*[currentlyOn]");
    Input(lightsPowerUsage).imports("./*[powerUsage]");
    Output(currentlyOn).help("Is any growth light on?");
    Output(powerUsage).help("Sum of growth lights' power usages");
}

void GrowthLights::update() {
    currentlyOn = false;
    powerUsage = 0.;
    for (bool on : lightsCurrentlyOn)
        currentlyOn = (currentlyOn || on);
    for (double usage : lightsPowerUsage)
        powerUsage += usage;
}

} //namespace

