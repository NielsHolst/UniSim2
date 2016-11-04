/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "water_viscosity.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (WaterViscosity)
	
WaterViscosity::WaterViscosity(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(temperature).equals(25.);     //average air temperature in the month of application, degrees celsius

    Output(value);    //viscosity of water (centipoise), between 0 - 50 degrees celsius
}

void WaterViscosity::reset() {

    update();

}

void WaterViscosity::update() {

    double x = 1.337 + temperature*0.001705;
    double y = 1. + temperature*0.01849;
    value = pow(x/y, 2);

}

} //namespace

