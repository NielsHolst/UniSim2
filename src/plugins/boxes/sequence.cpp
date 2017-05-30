#include <base/exception.h>
#include <base/publish.h>
#include "sequence.h"

using namespace base;

namespace boxes {

PUBLISH(Sequence)

Sequence::Sequence(QString name, QObject *parent)
    : Box(name, parent)
{
    help("produces a sequence of numbers in a [min,max] range");
    Input(by).equals("update").help("The sequence value can either be updated at 'reset' or 'update'");
    Input(min).equals(0).help("Minimum value in sequence");
    Input(max).equals(1).help("Maximum value in sequence");
    Input(counter).imports("/*[step]").help("To keep track of how far in the sequence we have come");
    Input(counterMax).imports("/*[steps]").help("Number of values in sequence");;
    Input(offset).equals(0).help("First value of counter, e.g. 0 or 1");
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
