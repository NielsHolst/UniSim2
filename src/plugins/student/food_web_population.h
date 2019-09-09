/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FOOD_WEB_POPULATION_H
#define FOOD_WEB_POPULATION_H
#include <base/food_web_box_base.h>

namespace student {

class FoodWebPopulation : public base::FoodWebBoxBase
{
public:
    FoodWebPopulation(QString name, QObject *parent);
    void reset();
private:
    // Input
    double initial, demandPerCap;
};

}

#endif
