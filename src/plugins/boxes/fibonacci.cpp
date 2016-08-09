#include <base/publish.h>
#include "fibonacci.h"

using namespace base;

namespace boxes {

PUBLISH(Fibonacci)

Fibonacci::Fibonacci(QString name, QObject *parent)
    : Box(name, parent)
{
    help("produces Fibonacci sequence");
    Output(value).help("Number in sequence");
}

void Fibonacci::reset() {
    previous = 0;
    value = 1;
}

void Fibonacci::update() {
    int value2 = value;
    value += previous;
    previous = value2;
}


}
