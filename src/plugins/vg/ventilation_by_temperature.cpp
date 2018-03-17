/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "ventilation_by_temperature.h"
#include "general.h"

using namespace base;

namespace vg {

PUBLISH(VentilationByTemperature)

VentilationByTemperature::VentilationByTemperature(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(coefficient).equals(0.1).help("Linear increase in ventilation rate with increasing temperature difference and vent area [m/K/h]");
    Input(indoorsTemperature).imports("indoors/temperature[value]]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(ventsEffectiveArea).imports("construction/shelter[ventsEffectiveArea]");
    Input(ventsMaxEffectiveArea).imports("construction/shelter[ventsMaxEffectiveArea]");
    Input(indoorsVolume).imports("construction/geometry[indoorsVolume]");
    Output(current).help("Current ventilation caused by outdoors wind [m3/m3/h]");
    Output(max).help("Max. possible ventilation given screens state, caused by outdoors wind [m3/m3/h]");
}

void VentilationByTemperature::reset() {
    max = current = 0.;
}

void VentilationByTemperature::update() {
    updateOutputs(indoorsTemperature, outdoorsTemperature);
}

void VentilationByTemperature::updateOutputs(double indoorsTemperature, double outdoorsTemperature)  {
    double dT = indoorsTemperature - outdoorsTemperature;
    if (dT < 0.) {
        max = current = 0.;
    }
    else {
        double perGroundArea = coefficient*dT/indoorsVolume; // m-2*h-1 = m/K/h*K/m3
        max = ventsMaxEffectiveArea*perGroundArea;           // h-1 = m2*m-2*h-1
        current = ventsEffectiveArea*perGroundArea;
    }
}

} //namespace

