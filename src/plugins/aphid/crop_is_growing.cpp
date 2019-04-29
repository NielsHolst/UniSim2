#include <base/publish.h>
#include "crop_is_growing.h"

using namespace base;

namespace aphid {

PUBLISH(CropIsGrowing)

CropIsGrowing::CropIsGrowing(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes if the crop has started to grow");
    Input(temperature).help("Daily average temperature").unit("oC");
    Output(value);
}

void CropIsGrowing::reset() {
    temperature_2 =
    temperature_3 =
    temperature_4 =
    temperature_5 = 0.;
    value=false;
}

void CropIsGrowing::update() {
    if (!value) {
        value = (temperature>0 && temperature_2>0 && temperature_3>0 && temperature_4>0 && temperature_5>0) ;
        temperature_5 = temperature_4;
        temperature_4 = temperature_3;
        temperature_3 = temperature_2;
        temperature_2 = temperature;
    }
}

}
