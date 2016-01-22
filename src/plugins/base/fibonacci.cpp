#include <boxes/publish.h>
#include "fibonacci.h"

using namespace boxes;

namespace base {

PUBLISH(Fibonacci)

Fibonacci::Fibonacci(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(Fibonacci);
    Output(value);
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
