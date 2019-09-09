/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "leaf_temperature.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(LeafTemperature)

LeafTemperature::LeafTemperature(QString name, QObject *parent)
	: Box(name, parent)
{
    help("models leaf temperature");
    Input(indoorsTemperature).imports("indoors/temperature[value]").unit("oC");
    Input(indoorsRh).imports("indoors/humidity[rh]").unit("[0;100]");
    Input(rsH2O).imports("../rs[rsH2O]").unit("s/m");
    Input(rbH2O).imports("../rb[rbH2O]").unit("s/m");
    Input(radiationAbsorbed).imports("../radiationAbsorbed[value]").unit("W/m2");
    Output(value).help("Leaf temperature").unit("oC");
}

void LeafTemperature::reset() {
    value = indoorsTemperature;
}

void LeafTemperature::update() {
    double s = svpSlope(indoorsTemperature),
           psatu = svp(indoorsTemperature),
           pgh = vpFromRh(indoorsTemperature, indoorsRh),
           Tgh3 = p3(indoorsTemperature+T0);

    value = (1/RhoAir/CpAir*(rsH2O+rbH2O)*radiationAbsorbed - 1/Psychr*(psatu-pgh))
            /
            (1+(s/Psychr+ rsH2O/rbH2O+ 1/(RhoAir*CpAir/4/Sigma*Tgh3)*(rsH2O+rbH2O)))
            + indoorsTemperature;
    if (std::isnan(value))
        ThrowException("LeafTemperature is not a number").context(this);

    /* Thermal storage was neglible, max 1-2 W/m2
    double dt = 300,
           sla = 30,
           C = 300;
    dT = value-prevValue;
    thermalStorage = dT*lai/sla*C/dt;
    */
}


} //namespace

