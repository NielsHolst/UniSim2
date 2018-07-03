/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "outdoors.h"
#include <base/box_builder.h>
#include <base/phys_math.h>
#include <base/publish.h>

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(Outdoors)

Outdoors::Outdoors(QString name, QObject *parent)
	: Box(name, parent)
{
    help("delivers readings of ourdoors weather");
    Input(propParRadiation).equals(0.47).help("Proportion of PAR in sunlight irradiation [0;1]");
    Input(co2).equals(400.).help("Outdoors CO2 concentration [ppm]");
    Input(soilTemperature).equals(10).help("Outdoors soil temperature [oC]");
    Input(temperature).imports("./records[Tair]").help("Outdoors ambient temperature [oC]");
    Input(rh).imports("./records[Rhair]").help("Outdoors ambient relative humidity [0;100]");
    Input(radiation).imports("./records[GlobRad]").help("Total sunlight irradiation [W/m2]");
    Input(diffuseRadiation).importsMaybe("./records[DifRad]").help("Diffuse component of sunlight irradiation [W/m2]");
    Input(windSpeed).imports("./records[Windspeed]").help("Outdoors wind speed [m/s]");
    Input(skyTemperature).importsMaybe("./records[Tsky]").help("Sky temperature [oC]");
    Output(parRadiation).help("PAR component of sunlight irradiation [W/m2]");
    Output(directRadiation).help("Direct component of sunlight irradiation [W/m2]");
    Output(propDirectRadiation).help("Proportion of direct component of sunlight irradiation [0;1]");
    Output(ah).help("Outdoors absolute humidity [kg/m3]");
    Output(sh).help("Outdoors specific humidity [Kg/kg]");
}

void Outdoors::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./records"))
        builder.
        box("Records").name("records").
            port("fileName").equals("input/sel_dk.txt").
        endbox();
}

void Outdoors::reset() {
    update();
}

void Outdoors::update() {
    parRadiation = propParRadiation*radiation;
    directRadiation = radiation - diffuseRadiation;
    propDirectRadiation = div0(directRadiation, radiation);
    ah = ahFromRh(temperature, rh);
    sh = shFromRh(temperature, rh);
}

} //namespace
