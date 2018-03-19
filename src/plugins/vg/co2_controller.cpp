/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "general.h"
#include <base/publish.h>
#include "co2_controller.h"

using std::min;
using namespace base;

namespace vg {

PUBLISH(Co2Controller)

Co2Controller::Co2Controller(QString name, QObject *parent)
	: Box(name, parent)
{
    help("Computes the rate of CO2 enrichment");
    Input(minCo2).imports("setpoints/co2/minimum[signal]");
    Input(maxCo2).imports("setpoints/co2/maximum[signal]");
    Input(indoorsCo2).imports("indoors/co2[value]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(injectionRate).equals(4.5).help("Rate of CO2 injection [g/m2/h]");
    Output(signal).help("Rate of CO2 enrichment [g/m2/h]");
}

void Co2Controller::reset() {
    signal = 0.;
    on = false;
}

void Co2Controller::update() {
    if (indoorsCo2 > maxCo2)
        on = false;
    else if (indoorsCo2 < minCo2)
        on = true;
    signal = on ? injectionRate : 0.;
}

} //namespace
