#include <base/exception.h>
#include <base/publish.h>
#include "sequence.h"

using namespace base;

namespace boxes {

PUBLISH(Sequence)

Sequence::Sequence(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(counter).imports("/*[step]");
    Input(counterMax).imports("/*[steps]");
    Input(min).equals(0);
    Input(max).equals(1);
    Output(value);
}

void Sequence::reset() {
    if (counterMax < 1)
        ThrowException("'counterMax' must be > 0").value(counterMax).context(this);
    update();
}

void Sequence::update() {
    // Make certain that final 'value' is exactly at 'max'
    value = (counter == counterMax) ? max : min + (max - min)*counter/counterMax;
}



}
