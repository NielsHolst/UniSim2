#include <base/publish.h>
#include "day_length_scale.h"

using namespace base;

namespace student {

PUBLISH(DayLengthScale)

DayLengthScale::DayLengthScale(QString name, QObject *parent)
    : Box(name, parent)
{
    help("scales a value by day length");
    Input(dayLength).equals(12).help("Length of day (hours)");
    Input(valueToScale).equals(100).help("Pending");
    Output(value);
}

void DayLengthScale::reset() {
    update();
}

void DayLengthScale::update() {
    value = valueToScale*dayLength/24.;
}


}
