/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "food_web_population.h"

using namespace base;

namespace student {

PUBLISH(FoodWebPopulation)

FoodWebPopulation::FoodWebPopulation(QString name, QObject *parent)
    : FoodWebBoxBase(name, parent)
{
    Input(initial).equals(100).help("Initial density");
    Input(demandPerCap).help("Per capita demand rate");
}

void FoodWebPopulation::reset() {
    density = initial;
    demand = density*demandPerCap;
}

}
