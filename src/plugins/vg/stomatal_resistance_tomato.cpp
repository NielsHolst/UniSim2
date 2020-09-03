/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "stomatal_resistance_tomato.h"

using std::min;
using std::max;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(StomatalResistanceTomato)

StomatalResistanceTomato::StomatalResistanceTomato(QString name, QObject *parent)
    : StomatalResistanceBase(name, parent)
{
    help("computes stomatal resistance of tomato");
    Input(riH2Omin).equals(82.).help("Minimum resistance against H2O").unit("s/m");
    Input(lai).imports("crop/lai[value]",CA).unit("m2/m2");
    Input(temperature).imports("../temperature[value]",CA).unit("oC");
    Input(indoorsTemperature).imports("indoors/temperature[value]value]",CA).unit("oC");
    Input(rh).imports("indoors/humidity[rh]",CA).unit("[0;100]");
    Input(radiationAbsorbed).imports("../radiationAbsorbed[value]",CA).unit("W/m2");
    Input(co2).imports("indoors/co2[value]",CA).unit("ppm");
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
    return riH2Omin*fRadiation*fTemperature*fCO2*fHumidity;
}


} //namespace

