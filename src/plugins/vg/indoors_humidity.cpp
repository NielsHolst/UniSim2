/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "general.h"
#include "indoors_humidity.h"
#include <base/publish.h>
#include "vapour_flux_base.h"

using namespace std;
using namespace base;

namespace vg {
	
PUBLISH(IndoorsHumidity)

IndoorsHumidity::IndoorsHumidity(QString name, QObject *parent)
	: Box(name, parent)
{
    help("models indoors humidity");
    Input(conductance).imports("indoors/total/vapourFlux[conductance]");
    Input(vapourFlux).imports("indoors/total/vapourFlux[vapourFlux]");
    Input(gain).imports("indoors/total/vapourFlux[gain]");
    Input(temperature).imports("indoors/temperature[value]");
    Input(height).imports("geometry[indoorsAverageHeight]");
    Input(timeStep).imports("calendar[timeStepSecs]");

    Output(rh).help("Indoors relative humidity [0;100]");
    Output(ah).help("Indoors absolute humidity [kg/m3]");
    Output(ahEq).help("Asymptotic (with time) equilibrium absolute humidity [kg/m3]");
    Output(timeConstant).help("Time constant for integrating netVapourFlux [s-1]");
    Output(surplusAh).help("Integration inaccuracy that was rounded off [kg/m3]");
    Output(netVapourFlux).help("Total water vapour flux in/out (+/-) of the greenhouse [kg/m2/s]");
}

void IndoorsHumidity::reset() {
    tick = 0;
    rh = 70.;
    ah = ahEq = ahFromRh(temperature, rh);
    netVapourFlux = timeConstant = surplusAh = 0.;
}

void IndoorsHumidity::update() {
    // Keep humidity constant for the first few time steps to stabilise overall model state
    if (tick++ < 10) return;
    double prevAh = ah;
    if (conductance > 0. && gain > 0.) {
        ahEq = gain/conductance;
        timeConstant = height/gain;
        ah = ahEq - (ahEq-ah)*exp(-timeStep/timeConstant);
    }
    else {
        ahEq = ah;
        timeConstant = 0.;
    }

    double indoorsSah = sah(temperature);
    if (ah > indoorsSah) {
        ah = indoorsSah;
        surplusAh = ah - indoorsSah;
    }
    else {
        surplusAh = 0.;
    }

    rh = rhFromAh(temperature, ah);
    netVapourFlux = (ah - prevAh)*height/timeStep; // kg/m2/s = kg/m3 * m3/m2 / s
}

} //namespace
