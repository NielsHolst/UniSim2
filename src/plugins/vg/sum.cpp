/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
    Input(inputs);
    Output(value);
}

void Sum::initialize() {
    ports.clear();
    for (QString input : inputs)
        ports << Path(input).resolveMany<Port>();
}

void Sum::update() {
    value = 0;
    for (Port *port : ports)
        value += port->value<double>();
}

} //namespace

