#include <base/publish.h>
#include "food_web_box.h"

using namespace base;

namespace boxes {

PUBLISH(FoodWebBox)

FoodWebBox::FoodWebBox(QString name, QObject *parent)
    : FoodWebBoxBase(name, parent)
{
    help("basic box for FoodWeb models");
}

}
