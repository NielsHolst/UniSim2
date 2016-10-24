/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "viscosity_of_water.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (viscosityofwater)
	
viscosityofwater::viscosityofwater(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(Tair).equals(25.);     //average air temperature in the month of application, degrees celsius

    Output(viscosity);    //viscosity of water (centipoise), between 0 - 50 degrees celsius
}

void viscosityofwater::reset() {

    viscosity = 0.;

}

void viscosityofwater::update() {

    double x = 1.337370563567217 + Tair * 0.001705494588353515;
    double y = 1. + Tair *0.01848546386356825;
    viscosity = pow(x/y, 2);

}

} //namespace

