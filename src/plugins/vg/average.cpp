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
#include "average.h"

using namespace base;

namespace vg {

PUBLISH(Average)

Average::Average(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes average of inputs");
    Input(inputs).help("Input values");
    Output(value).help("Average of inputs");
}

void Average::initialize() {
    ports.clear();
    for (QString input : inputs)
        ports << findMany<Port>(input);
//        ports << Path(input).resolveMany<Port>();
}

void Average::update() {
    if (ports.isEmpty())
        return;
    value = 0;
    for (Port *port : ports)
        value += port->value<double>();
    value /= ports.size();
}

} //namespace

