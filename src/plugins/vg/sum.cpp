/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/general.h>
#include <base/path.h>
#include <base/publish.h>
#include <base/port.h>
#include "sum.h"

using namespace base;

namespace vg {

PUBLISH(Sum)

Sum::Sum(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes sum of inputs");
    Input(inputs).help("Input values");
    Output(value).help("Sum of inputs");
}

void Sum::initialize() {
    ports.clear();
    for (QString input : inputs) {
        QVector<base::Port*> foundPorts = findMany<Port>(input);
        if (foundPorts.isEmpty())
            ThrowException("Input port not found").value(input).context(this);
        ports << foundPorts;
    }
}

void Sum::update() {
    value = 0;
    for (Port *port : ports)
        value += port->value<double>();
}

} //namespace

