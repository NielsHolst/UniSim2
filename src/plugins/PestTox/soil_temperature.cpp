/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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

