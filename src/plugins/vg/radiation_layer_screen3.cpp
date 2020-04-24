/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "radiation_layer_screen3.h"

using namespace base;

namespace vg {

PUBLISH(RadiationLayerScreen3)

RadiationLayerScreen3::RadiationLayerScreen3(QString name, QObject *parent)
    : RadiationLayerShelter(name, parent, "shelter/*/screens/layer3")
{
    help("holds the combined radiative parameters of screen layer 3");
}

void RadiationLayerScreen3::update() {
    updateRadiativeProperties();
    updateConvectiveProperties();
}

} //namespace

