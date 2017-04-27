/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "soil_temperature.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (SoilTemperature)
	
SoilTemperature::SoilTemperature(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(Tmin).imports("weather[Tmin]");
    Input(Tmax).imports("weather[Tmax]");
    Input(dayLength).imports("calendar[dayLength]");

    Output(value);
}

void SoilTemperature::reset() {

    update();
}

void SoilTemperature::update() {

    value = dayLength/24.*Tmax + (1.- dayLength/24.)*Tmin;
}

} //namespace

