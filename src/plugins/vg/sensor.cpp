/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "sensor.h"

using namespace base;

namespace vg {

PUBLISH(Sensor)

Sensor::Sensor(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes average of inputs");
    Input(indoorsTemperatureIn).equals(missing);
    Input(indoorsRhIn).equals(missing);
    Input(indoorsCo2In).equals(missing);
    Input(indoorsWindspeedIn).equals(missing);
    Input(outdoorsTemperatureIn).equals(missing);
    Input(outdoorsRhIn).equals(missing);
    Input(outdoorsCo2In).equals(missing);
    Input(outdoorsGlobalRadiationIn).equals(missing);
    Input(outdoorsWindSpeedIn).equals(missing);
    Input(soilTemperatureIn).equals(missing);
    Input(sinb).imports("calendar[sinb]");
    Output(indoorsTemperature);
    Output(indoorsRh);
    Output(indoorsAh);
    Output(indoorsCo2);
    Output(indoorsWindspeed);
    Output(outdoorsTemperature);
    Output(outdoorsRh);
    Output(outdoorsCo2);
    Output(outdoorsGlobalRadiation);
    Output(outdoorsWindSpeed);
    Output(soilTemperature);
}

void Sensor::reset() {
    update();
}

void Sensor::update() {
    // Outdoors
    outdoorsTemperature = isMissing(outdoorsTemperatureIn) ? 15. : outdoorsTemperatureIn;
    outdoorsRh = isMissing(outdoorsRhIn) ? 60. : outdoorsRhIn;
    outdoorsCo2 = isMissing(outdoorsCo2In) ? 405. : outdoorsCo2In;
    outdoorsGlobalRadiation = isMissing(outdoorsGlobalRadiationIn) ?
                estimateGlobalRadiation() : outdoorsGlobalRadiationIn;
    outdoorsWindSpeed = isMissing(outdoorsWindSpeedIn) ? 4. : outdoorsWindSpeedIn;
    soilTemperature = isMissing(soilTemperatureIn) ? 10. : soilTemperatureIn;
    // Indoors
    indoorsTemperature = isMissing(indoorsTemperatureIn) ? 20. : indoorsTemperatureIn;
    indoorsRh = isMissing(indoorsRhIn) ? outdoorsRh : indoorsRhIn;
    indoorsAh = phys_math::ahFromRh(indoorsTemperature, indoorsRh);
    indoorsCo2 = isMissing(indoorsCo2In) ? 800. : indoorsCo2In;
    indoorsWindspeed = isMissing(indoorsWindspeedIn) ? 0.1 : indoorsWindspeedIn;
}

bool Sensor::isMissing(double value) const {
    return value==missing;
}

double Sensor::estimateGlobalRadiation() const {
    const double &T(outdoorsTemperature);
    double y = 0.1802 + 13.4796*sinb + 0.0102*T - 10.9866*sinb*sinb + 0.00043*T*T
            - 0.0598*sinb*T + 0.005115*sinb*sinb*T*T;
    y = y*y*y*y;
    return (y > 750.) ? 750 : y;

}

} //namespace
