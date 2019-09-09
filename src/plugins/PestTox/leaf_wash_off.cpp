/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "leaf_wash_off.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (LeafWashOff)
	
LeafWashOff::LeafWashOff(QString name, QObject *parent)
    : LossRate(name, parent)
{
    Input(rainfall).imports("weather[Rain]").help("Daily rainfall (mm)");
    Input(washOffCoefficient).help("Wash-off coefficient (mm-1)");
}

double LeafWashOff::computeInstantaneous() {
    double dailyRate = 1. - exp(-washOffCoefficient*rainfall);  // per day
    return -log(1. - dailyRate)/24./3600.;  // per s
}

} //namespace

