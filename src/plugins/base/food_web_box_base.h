#ifndef BASE_FOOD_WEB_BOX_BASE
#define BASE_FOOD_WEB_BOX_BASE
#include "box.h"

namespace base {

class FoodWebBoxBase : public base::Box
{
public: 
    FoodWebBoxBase(QString name, QObject *parent);
protected:
    // Inputs
    double density, demand,
        loss, supply,
        supplyDemandRatio, lossRatio;
};

} //namespace
#endif
