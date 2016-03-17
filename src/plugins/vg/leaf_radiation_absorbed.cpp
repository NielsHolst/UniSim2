/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/exception.h>
#include <base/path.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "general.h"
#include "leaf_radiation_absorbed.h"

using std::max;
using namespace base;

namespace vg {

PUBLISH(LeafRadiationAbsorbed)


/*! \class LeafRadiationAbsorbed
 * \brief Calculates absorption of short- and long-waved radiation
 *
 * The model automatically looks up heating pipes.
 *
 * Inputs
 * ------
 * - _kLw_ is the extinction coefficent for long-waved radiation [0;1]
 * - _emissivity_ is the emissivity of leaves for long-waved radiation [0;1]
 * - _xGaussLowerside_ is the coefficient for Gaussian integration from the lower side of the canopy [0;1]
 * - _wGaussLowerside_ is the weihgt for Gaussian integration from the lower side of the canopy [0;1]
 * - _xGaussUpperside_ is the coefficient for Gaussian integration from the upper side of the canopy [0;1]
 * - _wGaussUpperside_ is the weihgt for Gaussian integration from the upper side of the canopy [0;1]
 * - _lai_ is the crop leaf area index [-]
 * - _indoorsLight_ is total the intensity of light indoors [W/m<SUP>2</SUP>]
 * - _lightAbsorptivity_ is the proportion of indoors light captured by this leaf layer [0;1]
 * - _growthLightLw_ is the long wave emission from growth lights [W/m<SUP>2</SUP>]
 * - _growthLightViewFactor_ is the view factor of growth lights as seen from this leaf layer [0;1]
 * - _floorTemperature_ is the floor temperature [<SUP>o</SUP>C]
 * - _floorEmissivity_ is floor emissivity [0;1]
 * - _leafTemperature_ is the leaf temperature [<SUP>o</SUP>C]
 * - _coverTemperature_ is the cover temperature [<SUP>o</SUP>C]
 * - _screensTemperature_ is the temperature of screens [<SUP>o</SUP>C]
 * - _screensMaxState_ is the maximum screen state [0;1]
 * - _shelterOutgoingLwAbsorptivity_ is the absorptivity for long-waved radiation of the greenhouse shelter [0;1]
 * - _coverPerGroundArea_ is the total cover area per ground area [m<SUP>2</SUP> cover/m<SUP>2</SUP> ground]
 *
 * Outputs
 * ------
 *  - _lightAbsorbed_ is the flux of light absorbed by this leaf layer [W/m<SUP>2</SUP>]
 *  - _heatingAbsorbed_ is the flux of heating absorbed by this leaf layer [W/m<SUP>2</SUP>]
 *  - _growthLightLwAbsorbed_ is the flux of long-waved radiation absorbed by this leaf layer from the growth lights [W/m<SUP>2</SUP>]
 *  - _floorLwAbsorbed_ is the flux of long-waved radiation absorbed by this leaf layer from the floor [W/m<SUP>2</SUP>]
 *  - _shelterLoss_ is the flux of long-waved radiation lost from this leaf layer to the shelter [W/m<SUP>2</SUP>]
 *  - _value_ is the energy flux absorbed by this leaf layer [W/m<SUP>2</SUP>]
 */

LeafRadiationAbsorbed::LeafRadiationAbsorbed(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(kLw).equals(0.8);
    Input(emissivity).equals(0.8);
    Input(xGaussLowerside).imports("..[xGaussLowerside]");
    Input(wGaussLowerside).imports("..[wGaussLowerside]");
    Input(xGaussUpperside).imports("..[xGaussUpperside]");
    Input(wGaussUpperside).imports("..[wGaussUpperside]");
    Input(lai).imports("crop/lai[value]");
    Input(indoorsLight).imports("indoors/light[total]");
    Input(lightAbsorptivity).imports("../photosynthesis[absorptivity]");
    Input(growthLightLw).imports("actuators/growthLights[longWaveEmission]");
    Input(growthLightViewFactor).equals(1.);
    Input(floorTemperature).imports("energyFlux/floor[temperature]");
    Input(floorEmissivity).imports("energyFlux/floor[emissivity]");
    Input(leafTemperature).imports("../temperature[value]");
    Input(coverTemperature).imports("given/energyFlux/shelter[coverTemperature]");
    Input(screensTemperature).imports("given/energyFlux/shelter[screensTemperature]");
    Input(screensMaxState).imports("construction/shelters[screensMaxState]");
    Input(shelterOutgoingLwAbsorptivity).imports("construction/shelters[outgoingLwAbsorptivity]");
    Input(coverPerGroundArea).imports("construction/geometry[coverPerGroundArea]");

    Output(lightAbsorbed);
    Output(heatingAbsorbed);
    Output(growthLightLwAbsorbed);
    Output(floorLwAbsorbed);
    Output(shelterLoss);
    Output(value);
}

void LeafRadiationAbsorbed::initialize() {
    QVector<Box*> pipes = Path("actuators/heating/pipes/*").resolveMany<Box>(this);
    for (Box *pipe : pipes) {
        pipeInfos << PipeInfo {
                        pipe->port("length")->valuePtr<double>(),
                        pipe->port("diameter")->valuePtr<double>(),
                        pipe->port("temperature")->valuePtr<double>(),
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

void LeafRadiationAbsorbed::setHeatingAbsorbed() {
    heatingAbsorbed = 0;
    for (PipeInfo pi : pipeInfos) {
        if (*pi.temperature > leafTemperature) {
            double em = jointEmissivity(emissivity, *pi.emissivity);
            heatingAbsorbed += Sigma*em*(p4K(*pi.temperature) - p4K(leafTemperature))*lwTransmissionLowerside*pi.area();
        }
    }
}


} //namespace

