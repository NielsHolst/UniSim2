/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "ventilation_by_wind.h"
#include "general.h"
#include <base/publish.h>

using std::min;
using namespace base;

namespace vg {

PUBLISH(VentilationByWind)

VentilationByWind::VentilationByWind(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(coefficient).equals(0.1).help("Linear increase in ventilation rate with increasing wind speed and vent area [unitless]");
    Input(windSpeed).imports("outdoors[windSpeed]");
    Input(ventsEffectiveArea).imports("construction/shelter[ventsEffectiveArea]");
    Input(ventsMaxEffectiveArea).imports("construction/shelter[ventsMaxEffectiveArea]");
    Input(indoorsVolume).imports("construction/geometry[indoorsVolume]");
    Output(current).help("Current ventilation caused by outdoors wind [m3/m3/h]");
    Output(max).help("Max. possible ventilation given screens state, caused by outdoors wind [m3/m3/h]");
}

void VentilationByWind::reset() {
    updateOutputs(4.);
}

void VentilationByWind::update() {
    updateOutputs(windSpeed);
}

void VentilationByWind::updateOutputs(double windSpeed) {
    double perGroundArea = coefficient*windSpeed*3600./indoorsVolume; // m-2*h-1 = 1*m/s*s/h/m3
    max = ventsMaxEffectiveArea*perGroundArea;                        // h-1 = m2*m-2*h-1
    current = ventsEffectiveArea*perGroundArea;
}

} //namespace

