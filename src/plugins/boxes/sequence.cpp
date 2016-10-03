#include <base/exception.h>
#include <base/publish.h>
#include "sequence.h"

using namespace base;

namespace boxes {

PUBLISH(Sequence)

Sequence::Sequence(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(by).equals("update");
    Input(counter).imports("/*[step]");
    Input(counterMax).imports("/*[steps]");
    Input(offset).equals(0);
    Input(min).equals(0);
    Input(max).equals(1);
    Output(value);
}

void Sequence::amend() {
    if (by == "reset") {
        port("counter")->imports("/*[iteration]");
        port("counterMax")->imports("/*[iterations]");
        port("value")->noReset();
        offset = 1;
    }
    else if (by != "update")
        ThrowException("'by' must be either 'reset' or 'update'").value(by);
}

void Sequence::reset() {
    updateValue();
}

void Sequence::update() {
    updateValue();
}

void Sequence::updateValue() {
    if (counterMax < 1)
        ThrowException("'counterMax' must be > 0").value(counterMax).context(this);
    value = (counter == counterMax) ? max : min + (max - min + offset)*(counter - offset)/counterMax;
}


}
