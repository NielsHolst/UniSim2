/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "indoors_temperature.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(IndoorsTemperature)

IndoorsTemperature::IndoorsTemperature(QString name, QObject *parent)
    : Box(name, parent)
{
    help("models indoors temperature");
    Input(resetValue).equals(20.).help("Indoors temperature when model is reset").unit("oC");
    Input(energyFlux).help("Energy flux dissipated into greenhouse air").unit("W/m2");
    Input(baseTemperature).imports(".[value]").unit("oC");
    Input(height).imports("geometry[indoorsAverageHeight]",CA).unit("m");
    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");
    Output(value).help("Indoors temperature").unit("oC");
}

void IndoorsTemperature::reset() {
    value = resetValue;
    tick = 0;
}

void IndoorsTemperature::update() {
    // Keep temperature constant for the first few time steps to stabilise overall model state
    if (tick++ < 10) return;
    double Cair = height*RhoAir*CpAir;               // J/m2/K = m * kg/m3 * J/kg/K
    value = baseTemperature + energyFlux*timeStep/Cair;  // K = W/m2 * s / (J/m2/K)
}

} //namespace
