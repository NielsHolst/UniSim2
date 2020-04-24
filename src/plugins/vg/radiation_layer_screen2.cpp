/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "radiation_layer_screen2.h"

using namespace base;

namespace vg {

PUBLISH(RadiationLayerScreen2)

RadiationLayerScreen2::RadiationLayerScreen2(QString name, QObject *parent)
    : RadiationLayerShelter(name, parent, "shelter/*/screens/layer2")
{
    help("holds the combined radiative parameters of screen layer 2");
}

void RadiationLayerScreen2::update() {
    updateRadiativeProperties();
    updateConvectiveProperties();
}

} //namespace

