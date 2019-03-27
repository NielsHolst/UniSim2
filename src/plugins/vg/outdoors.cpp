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
    Input(sunlightPhotonCoef).equals(4.6).help("Number of PAR photons in sunlight energy").unit("micromole/J");
    Input(co2).equals(400.).help("Outdoors CO2 concentration").unit("ppm");
    Input(temperature).imports("./records[Tair]").help("Outdoors ambient temperature").unit("oC");
    Input(rh).imports("./records[Rhair]").help("Outdoors ambient relative humidity").unit("[0;100]");
    Input(radiation).imports("./records[GlobRad]").help("Total sunlight irradiation").unit("W/m2");
    Input(diffuseRadiation).importsMaybe("./records[DifRad]").help("Diffuse component of sunlight irradiation").unit("W/m2");
    Input(windSpeed).imports("./records[Windspeed]").help("Outdoors wind speed").unit("m/s");
    Input(skyTemperature).importsMaybe("./records[Tsky]").help("Sky temperature").unit("oC");
    Output(directRadiation).help("Direct component of sunlight irradiation").unit("W/m2");
    Output(propDirectRadiation).help("Proportion of direct component of sunlight irradiation").unit("[0;1]");
    Output(ah).help("Outdoors absolute humidity").unit("kg/m3");
    Output(sh).help("Outdoors specific humidity").unit("kg/kg");
    Output(soilTemperature).imports("./soilTemperature[value]").unit("oC");
}

void Outdoors::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./records"))
        builder.
        box("Records").name("records").
            port("fileName").equals("input/sel_dk.txt").
            port("ignoreYear").equals(true).
        endbox();
    if (!findMaybeOne<Box>("./soilTemperature"))
        builder.
        box("Accumulator").name("soilTemperature").
            port("initial").imports("./initial[value]").
            port("change").imports("./controller[controlVariable]").
            box("Hump").name("initial").
                port("x").imports("calendar[dayOfYear]").
                port("x0").equals(70).
                port("x1").equals(365).
                port("ymin").equals(0).
                port("ymax").equals(16).
            endbox().
            box("PidController").name("controller").
                port("sensedValue").imports("..[value]").
                port("desiredValue").imports("outdoors[temperature]").
                port("Kprop").equals(0.5e-4).
            endbox().
        endbox();
}

void Outdoors::reset() {
    update();
}

void Outdoors::update() {
    directRadiation = radiation - diffuseRadiation;
    propDirectRadiation = div0(directRadiation, radiation);
    ah = ahFromRh(temperature, rh);
    sh = shFromRh(temperature, rh);
}

} //namespace
