#include <base/publish.h>
#include "day_length_scale.h"

using namespace base;

namespace student {

PUBLISH(DayLengthScale)

DayLengthScale::DayLengthScale(QString name, QObject *parent)
    : Box(name, parent)
{
    help("scales a value by day length");
    Input(dayLength).equals(12).unit("h").help("Length of day");
    Input(valueToScale).equals(100).
        help("Value that will be scaled by relative day length");
    Output(value).help("Scaled value");
}

void DayLengthScale::reset() {
    update();
}

void DayLengthScale::update() {
    value = valueToScale*dayLength/24.;
}

}
