/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "general.h"
#include "indoors_temperature.h"
#include <base/publish.h>

using namespace base;

namespace vg {
	
PUBLISH(IndoorsTemperature)

IndoorsTemperature::IndoorsTemperature(QString name, QObject *parent)
    : Box(name, parent), _buffer(&_data)
{
    help("models indoors temperature");
    Input(resetValue).equals(20.).help("Indoors temperature when model is reset [oC]");
    Input(energyFlux).help("Energy flux dissipated into greenhouse air [W/m2]");
    Input(baseTemperature).imports(".[value]");
    Input(height).imports("geometry[indoorsAverageHeight]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(averageTimeSpan).equals(15.).help("Time span over which to calculate average [min]");
    Output(value).help("Indoors temperature [oC]");
    Output(average).help("Indoors temperature averages over averageTimeSpan [oC]");
    Output(averageNumber).help("Number of temperatures used to compute average");
}

void IndoorsTemperature::reset() {
    value = average = resetValue;
    tick = 0;
    double dt = timeStep/60.;
    averageNumber = static_cast<int>(floor(averageTimeSpan/dt));
    _buffer.resize(averageNumber);
    for (int i=0; i<averageNumber; ++i)
        _buffer.push(resetValue);
}

void IndoorsTemperature::update() {
    // Keep temperature constant for the first few time steps to stabilise overall model state
    if (tick++ < 10) return;
    double Cair = height*RhoAir*CpAir;               // J/m2/K = m * kg/m3 * J/kg/K
    value = baseTemperature + energyFlux*timeStep/Cair;  // K = W/m2 * s / (J/m2/K)
    // Compute average
    _buffer.push(value);
    average = _buffer.average();
}

} //namespace
