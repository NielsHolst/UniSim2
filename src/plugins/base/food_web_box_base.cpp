/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "food_web_box_base.h"

namespace base {

FoodWebBoxBase::FoodWebBoxBase(QString name, QObject *parent)
    : Box(name, parent) {
    Class(FoodWebBoxBase);
    help("provides a base class for FoodWeb models");
    Input(supply).help("Trophic supply <= demand");
    Input(loss).help("Trophic loss");
    Input(supplyDemandRatio).help("Supply/Demand ratio; zero if demand is zero");
    Input(lossRatio).help("Loss/Density ratio; zero if density is zero");
    Input(density).help("Density in food web");
    Input(demand).help("Demand towards supply");
}

} //namespace

