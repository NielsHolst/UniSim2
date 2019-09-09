/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "fibonacci.h"

using namespace base;

namespace student {

PUBLISH(Fibonacci)

Fibonacci::Fibonacci(QString name, QObject *parent)
    : Box(name, parent)
{
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
