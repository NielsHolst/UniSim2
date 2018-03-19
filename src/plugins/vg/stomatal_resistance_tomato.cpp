/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/publish.h>
#include "general.h"
#include "stomatal_resistance_tomato.h"

using std::min;
using std::max;
using namespace base;

namespace vg {

PUBLISH(StomatalResistanceTomato)

StomatalResistanceTomato::StomatalResistanceTomato(QString name, QObject *parent)
    : StomatalResistanceBase(name, parent)
{
    help("computes stomatal resistance of tomato");
    Input(riH2Omin).equals(82.).help("Minimum resistance against H2O");
    Input(lai).imports("crop/lai[value]");
    Input(temperature).imports("../temperature[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(rh).imports("indoors/humidity[rh]");
    Input(radiationAbsorbed).imports("../radiationAbsorbed[value]");
    Input(co2).imports("indoors/co2[value]");
}

double StomatalResistanceTomato::resetRsH2O() {
    return riH2Omin;
}

double StomatalResistanceTomato::updateRsH2O() {
    if (lai==0.) return riH2Omin;
    // Stanghellini (page 37, 87)
    // stomatal (internal) resistance to H2O [s/m] as a function of irrad, Temp, CO2, RH
    bool atNight = (radiationAbsorbed < 3);
    double vpd = vpdFromRh(indoorsTemperature, rh),
           radAbs = max(radiationAbsorbed/(2*lai), 0.), // W/p2 leaf = W/m2 ground / (m2 leaf/m2 ground)
           fRadiation = (radAbs + 4.3)/(radAbs + 0.54),
           fTemperature = 1 + 2.2593e-2*sqr(temperature + T0 - 297.66),
           fCO2 = atNight ? 1 : min(1 + 6.08e-7*sqr(co2-200), 1.49),
           fHumidity = 4/pow((1 + 255*exp(-0.5427*vpd*0.01)), 0.25);
//    double test = riH2Omin*fRadiation*fTemperature*fCO2*fHumidity;
    return riH2Omin*fRadiation*fTemperature*fCO2*fHumidity;
}


} //namespace

