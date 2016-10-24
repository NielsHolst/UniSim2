/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
    Input(concentration).equals(1.);      //predicted concentration in the soil at time t (kg a.i/ha)
    Input(slope).equals(1.);              //scale the specific toxicity of the simulated pesticide or sigmoidal function (volume/amount)
    Input(ar).equals(1.);                  //application rate, L/ha

    Output(mortalityRate);         //per cent of eggs, tadpole, ....adult frog dies per day
    Output(C);                     //predicted soil concentration at time t (mg a.i/L)
    Output(survivalRate);          //fraction of eggs, tadpoles, ...adults frogs survived

}

void pesticideinducedmortalityrate::reset() {

    C = 0.;
    mortalityRate = 0.;
    survivalRate = 0.;

}

void pesticideinducedmortalityrate::update() {

    C = (concentration > 0) ? (concentration * 1000000.)/ar : 0;
    mortalityRate = (1./(1. + exp(-slope*(log(C) - log(LC50)))));
    survivalRate = (survivalRate < 0) ? 0 : (1.- mortalityRate);

}

} //namespace

