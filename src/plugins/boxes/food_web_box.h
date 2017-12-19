#ifndef FOOD_WEB_BOX_H
#define FOOD_WEB_BOX_H
#include <base/food_web_box_base.h>

namespace boxes {

class FoodWebBox : public base::FoodWebBoxBase
{
public:
    FoodWebBox(QString name, QObject *parent);
};

}

#endif
