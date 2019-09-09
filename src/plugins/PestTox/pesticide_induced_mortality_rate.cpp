/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "pesticide_induced_mortality_rate.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (pesticideinducedmortalityrate)
	
pesticideinducedmortalityrate::pesticideinducedmortalityrate(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(LC50).equals(0.);               //constant LC50 (mg a.i/L)
    Input(concentration).equals(0.);      //concentration in the water (mg a.i/L)
    Input(slope).equals(1.);              //scale the specific toxicity of the simulated pesticide or sigmoidal function (volume/amount)

    Output(mortalityRate);         //fraction of eggs, tadpole, ....adult frog dies per day
    Output(survivalRate);          //fraction of eggs, tadpoles, ...adults frogs survived per day

}

void pesticideinducedmortalityrate::reset() {

    mortalityRate = 0.;
    survivalRate = 0.;

}

void pesticideinducedmortalityrate::update() {

    mortalityRate = (1./(1. + exp(-slope*(log(concentration) - log(LC50)))));
    survivalRate = (survivalRate < 0) ? 0 : (1.- mortalityRate);

}

} //namespace

