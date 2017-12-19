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

