/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "general.h"
#include "indoors_co2.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(IndoorsCo2)

IndoorsCo2::IndoorsCo2(QString name, QObject *parent)
	: Box(name, parent)
{
    help("models indoors CO2 concentration");
    Input(outdoorsCo2).imports("outdoors[co2]");
    Input(airFlux).imports("total/airFlux[value]");
    Input(injectionRate).imports("controllers/co2[signal]");
    Input(assimilation).imports("crop/growth/Pg[value]");
    Input(averageHeight).imports("geometry[indoorsAverageHeight]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(value).help("Indoors CO2 concentration [ppm]");
}

void IndoorsCo2::reset() {
    value = outdoorsCo2;
}

namespace {
    double integrate(double a,  // injection rate - assimilation rate
                     double b,  // ventilation rate
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

