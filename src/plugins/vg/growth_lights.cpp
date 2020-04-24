/* Copyright 2005-2019 by
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
    : RadiationLayer(name, parent)
{
    help("sums the radiative emittance from several growth lights");
    port("parFluxDown")->imports("./*[parFluxDown]").transform(Sum);
    port("swFluxDown")->imports("./*[swFluxDown]").transform(Sum);
    port("lwFluxDown")->imports("./*[lwFluxDown]").transform(Sum);
    Output(currentlyOn).imports("./*[currentlyOn]").transform(Any).help("Is any growth light on?").unit("y|n");;
}

} //namespace

