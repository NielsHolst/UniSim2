/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_co2.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(IndoorsCo2)

/*! \class IndoorsCo2
 * \brief Keeps track of indoors CO2 concentraion
 *
 * Inputs
 * ------
 * - _outdoorsCo2_ is the CO<SUB>2</SUB> concentration outside the greenhouse [ppm]
 * - _airFlux_ is the ventilation rate [h<SUP>-1</SUP>]
 * - _injectionRate_ is the rate of CO<SUB>2</SUB> injection [g/m<SUP>2</SUP>/h]
 * - _assimilation_ is the rate of CO<SUB>2</SUB> uptake by the plants [g/m<SUP>2</SUP>/h]
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _timeStep_ is the duration of one integration time step [2]
 *
 * Output
 * ------
 * - _value_ is the CO<SUB>2</SUB> concentration inside the greenhouse [ppm]
 *
 */

IndoorsCo2::IndoorsCo2(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(outdoorsCo2).imports("outdoors[co2]");
    Input(airFlux).imports("total/airFlux[value]");
    Input(injectionRate).imports("controllers/co2[signal]");
    Input(assimilation).imports("crop/growth/Pg[value]");
    Input(averageHeight).imports("geometry[indoorsAverageHeight]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(value);
}

void IndoorsCo2::reset() {
    value = outdoorsCo2;
}

namespace {
    double integrate(double a,  // injection rate - assimilation rate
                     double b,  // ventilation rate)
                     double c,  // outdoors CO2
                     double y0, // initial indoors CO2
                     double dt) // time step
    {
        if (b==0)
            return y0 + a*dt;
        else
            return exp(-b*dt)*( (exp(b*dt)-1)*(a/b+c)+y0 );
    }
}

void IndoorsCo2::update() {
    value = integrate(
                (injectionRate-assimilation)/averageHeight/1.83e-3,   // ppm/h = g/m2/h / m / (g/m3/ppm)
                airFlux,
                outdoorsCo2,
                value,
                timeStep/3600.
                );
}

} //namespace

