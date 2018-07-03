/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "leaf_transpiration.h"
#include "vapour_flux_base.h"

using std::max;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(LeafTranspiration)

LeafTranspiration::LeafTranspiration(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    help("models leaf transpiration");
    Input(lai).imports("crop/lai[value]");
    Input(fractionPlantArea).imports("crop/lai[fractionPlantArea]");
    Input(indoorsAh).imports("indoors/humidity[ah]");
    Input(radiationAbsorbed).imports("../radiationAbsorbed[value]");
    Input(leafTemperature).imports("../temperature[value]");
    Input(rbH2O).imports("../rb[rbH2O]");
    Input(rsH2O).imports("../rs[rsH2O]");
    Output(leafAh).help("Leaf absolute humidity [kg/m3]");
}

void LeafTranspiration::reset() {
    leafAh = indoorsAh;
}

void LeafTranspiration::update() {
    if (lai == 0.) {
        conductance = vapourFlux = gain = 0.;
    }
    else {
        double laiNet = lai*fractionPlantArea,
               s = svpSlope(leafTemperature);
        conductance = 2*laiNet/((1 + s/Psychr)*rbH2O + rsH2O);
        leafAh = sah(leafTemperature) + s/Psychr*rbH2O/2/laiNet*radiationAbsorbed/LHe;
        vapourFlux = max(conductance*(leafAh - indoorsAh), 0.);
        gain = conductance*leafAh;
    }
}


} //namespace

