/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/path.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "leaf_radiation_absorbed.h"

using std::max;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(LeafRadiationAbsorbed)
LeafRadiationAbsorbed::LeafRadiationAbsorbed(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes leaf absorption of short and long waved radiation");
    Input(kLw).equals(0.8).help("Extinction coefficent for long-waved radiation [0;1]").unit("m2/m2");
    Input(emissivity).equals(0.8).help("Leaf emissivity for long-waved radiation [0;1]").unit("[0;1]");
    Input(xGaussLowerside).imports("..[xGaussLowerside]").unit("[0;1]");
    Input(wGaussLowerside).imports("..[wGaussLowerside]").unit("[0;1]");
    Input(xGaussUpperside).imports("..[xGaussUpperside]").unit("[0;1]");
    Input(wGaussUpperside).imports("..[wGaussUpperside]").unit("[0;1]");
    Input(lai).imports("crop/lai[value]").unit("m2/m2");
    Input(indoorsLight).imports("indoors/light[total]").unit("W/m2");
    Input(lightAbsorptivity).imports("../photosynthesis[absorptivity]").unit("[0;1]");
    Input(growthLightLw).imports("actuators/growthLights[longWaveIntensity]").unit("W/m2");
    Input(growthLightViewFactor).equals(1.).unit("[0;1]");
    Input(floorTemperature).imports("energyFlux/floor[temperature]").unit("oC");
    Input(floorEmissivity).imports("energyFlux/floor[emissivity]").unit("[0;1]");
    Input(leafTemperature).imports("../temperature[value]").unit("oC");
    Input(coverTemperature).imports("given/energyFlux/shelter[coverTemperature]").unit("oC");
    Input(screensTemperature).imports("given/energyFlux/shelter[screensTemperature]").unit("oC");
    Input(screensMaxState).imports("construction/shelter[screensMaxState]").unit("[0;1]");
    Input(shelterOutgoingLwAbsorptivity).imports("construction/shelter[outgoingLwAbsorptivity]").unit("W/m2");
    Input(coverPerGroundArea).imports("construction/geometry[coverPerGroundArea]").unit("m2/m2");
    Input(pipeInflowTemperature).imports("actuators/heating[value]").unit("oC");

    Output(lightAbsorbed).help("Light flux absorbed by this leaf layer").unit("W/m2");
    Output(heatingAbsorbed).help("Heating radiation flux absorbed by this leaf layer").unit("W/m2");
    Output(growthLightLwAbsorbed).help("Lamp thermal radiation flux absorbed by this leaf layer").unit("W/m2");
    Output(floorLwAbsorbed).help("Floor thermal radiation flux absorbed by this leaf layer").unit("W/m2");
    Output(shelterLoss).help("Thermal radiation lost to the shelter from this leaf layer").unit("W/m2");
    Output(value).help("Net energy flux absorbed by this leaf layer").unit("W/m2");
}

void LeafRadiationAbsorbed::initialize() {
    QVector<Box*> pipes = Path("actuators/heating/pipes/*").resolveMany<Box>(this);
    for (Box *pipe : pipes) {
        pipeInfos << PipeInfo {
                        pipe->port("density")->valuePtr<double>(),
                        pipe->port("diameter")->valuePtr<double>(),
                        pipe->port("emissivity")->valuePtr<double>()
                     };
    }
}

void LeafRadiationAbsorbed::update() {
    lwTransmissionLowerside = kLw*exp(-kLw*lai*xGaussLowerside)*wGaussLowerside*lai;
    lwTransmissionUpperside = kLw*exp(-kLw*lai*xGaussUpperside)*wGaussUpperside*lai;
    setLightAbsorbed();
    setGrowthLightLwAbsorbed();
    setFloorLwAbsorbed();
    setShelterLoss();
    setHeatingAbsorbed();
    value = lightAbsorbed + heatingAbsorbed + growthLightLwAbsorbed + floorLwAbsorbed - shelterLoss;
}

void LeafRadiationAbsorbed::setLightAbsorbed() {
    lightAbsorbed = lightAbsorptivity*indoorsLight;
}

void LeafRadiationAbsorbed::setGrowthLightLwAbsorbed() {
    // This is a shortcut. We should know the temperature and area of the lamps.
    growthLightLwAbsorbed = growthLightViewFactor*growthLightLw*lwTransmissionUpperside*emissivity;
}

void LeafRadiationAbsorbed::setFloorLwAbsorbed() {
    double em = jointEmissivity(emissivity, floorEmissivity);
    floorLwAbsorbed = Sigma*em*(p4K(floorTemperature) - p4K(leafTemperature))*lwTransmissionLowerside;
}

void LeafRadiationAbsorbed::setShelterLoss() {
    double
        em = jointEmissivity(emissivity, shelterOutgoingLwAbsorptivity),
        screensDiff = p4K(leafTemperature) - p4K(screensTemperature),
        coverDiff = p4K(leafTemperature) - p4K(coverTemperature);
    shelterLoss = Sigma*em*coverPerGroundArea*lwTransmissionUpperside*
                  (screensDiff*screensMaxState + coverDiff*(1-screensMaxState));
}

// Check this. Pipe heating to air already taken care of. At least integrate from inflow to outflow temperature.
void LeafRadiationAbsorbed::setHeatingAbsorbed() {
    heatingAbsorbed = 0;
    for (PipeInfo pi : pipeInfos) {
        if (pipeInflowTemperature > leafTemperature) {
            double em = jointEmissivity(emissivity, *pi.emissivity);
            heatingAbsorbed += Sigma*em*(p4K(pipeInflowTemperature) - p4K(leafTemperature))*lwTransmissionLowerside*pi.area();
        }
    }
}


} //namespace

