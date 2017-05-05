#include <base/publish.h>
#include "dev_rate.h"

using namespace base;

namespace cmbp {

PUBLISH(DevRate)

DevRate::DevRate(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(b1).help("Regression parameter");
    Input(b2).help("Regression parameter");
    Input(b3).help("Regression parameter");
    Input(b4).help("Regression parameter");
    Input(T).help("Ambient temperature (oC)");
    Output(value).help("Daily development rate");
}

void DevRate::reset() {
    update();
}

void DevRate::update() {
    value = exp(b1*T) - exp(b1*b2 - (b2-T)/b3) + b4;
}


}
