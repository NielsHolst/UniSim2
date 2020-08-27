/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "indoors_ventilation.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(IndoorsVentilation)

IndoorsVentilation::IndoorsVentilation(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes total air flux from outdoors to indoors");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(effectiveArea).imports("shelter/*/*[effectiveArea]").transform(Sum);
    Input(groundArea).imports("construction/geometry[groundArea]", CA);
    Input(volume).imports("construction/geometry[volume]", CA);
    Input(leakage).equals(1).help("Infiltration rate at a wind speed of 4 m/s").unit("/h");
    Input(windSpeed).imports("outdoors[windSpeed]", CA);
    Input(outdoorsTemperature).imports("outdoors[temperature]", CA);
    Input(indoorsTemperature).imports("indoors[temperature]", CA);
    Input(state).imports("actuators/ventilation[value]", CA);
    Input(windCoef).equals(50.).help("Proportionality of air flux with windspeed").unit("/h/(m/s)");
    Input(temperatureCoef).equals(14.).help("Proportionality of air flux with temperature diffence").unit("/h/K");
    Input(airTransmissivities).imports("shelter/*/screens/airTransmissivity[value]", CA).
            help("Air transmissivity through screens for each shelter face");
    Input(areas).imports("shelter/*/area[value]", CA).
            help("Area of each shelter face");
    Output(relative).help("Relative air flux").unit("/h");
    Output(absolute).help("Absolute air flux over this time step").unit("m3");
//    Output(dueToLeakage).help("Part of total air flux due to leakage").unit("/h");
//    Output(dueToVents).help("Part of total air flux due to vents").unit("/h");
    Output(airTransmissivity).help("Total net transmissivity of screens").unit("[0;1]");
    Output(relativeArea).help("Vent area relative to ground area, corrected for vent and screens states").unit("-");
    Output(fluxLeak).help("Air flux if leakage were the only cause").unit("/h");
    Output(fluxWind).help("Air flux through vents, if wind were the only cause").unit("/h");
    Output(fluxTemp).help("Air flux through vents, if temperature were the only cause").unit("/h");
}

void IndoorsVentilation::reset() {
    update();
}

void IndoorsVentilation::update() {
    airTransmissivity = vector_op::weightedAverage(airTransmissivities, areas, this);
    relativeArea  = state*effectiveArea/groundArea;
    fluxLeak = airTransmissivity*leakage*windSpeed/4.;
    fluxWind = airTransmissivity*relativeArea*windCoef*windSpeed;
    fluxTemp = airTransmissivity*relativeArea*temperatureCoef*std::max(indoorsTemperature-outdoorsTemperature, 0.);
//    double sum = fluxLeak+fluxWind+fluxTemp;
    relative = sqrt(p2(fluxLeak) + p2(fluxWind) + p2(fluxTemp));
    absolute = volume*(1. - exp(-relative*timeStep/3600.));
//    dueToLeakage = (sum > 0.) ? relative*fluxLeak/sum : 0.;
//    dueToVents = relative - dueToLeakage;
}

} //namespace

