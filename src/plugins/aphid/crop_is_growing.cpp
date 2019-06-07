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
    Input(T0).equals(5.).help("Threshold that triggers crop growth").unit("oC");
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
        value = (temperature>T0 && temperature_2>T0 && temperature_3>T0 && temperature_4>T0 && temperature_5>T0) ;
        temperature_5 = temperature_4;
        temperature_4 = temperature_3;
        temperature_3 = temperature_2;
        temperature_2 = temperature;
    }
}

}
