/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include "box.h"
#include "box_output.h"
#include "exception.h"
#include "general.h"
#include "port.h"

namespace base {

BoxOutput::BoxOutput(const Box *box, Format format)
    : _box(box), _format(format), _level(0)
{
}

QString BoxOutput::asText() {
    _output = "";
    addOutput(_box);
    // remove superflous newline at end
    int n = _output.size();
    if (n > 0)
        _output.remove(n-1, 1);
    return _output;
}

void BoxOutput::addOutput(const QObject *box) {
    _output += entry(box);
    auto children = box->findChildren<const QObject*>(QString(), Qt::FindDirectChildrenOnly);
    if (!children.isEmpty()) {
        _output += beginLevel();
        for (const QObject *child : children)
            addOutput(child);
        _output += endLevel();
    }
}

QString BoxOutput::entry(const QObject *object) {
    const Port *port = dynamic_cast<const Port*>(object);
    const Box *box = dynamic_cast<const Box*>(object);
    QString text = beforeEntry() + className(object) + " " + object->objectName();
    if (box)
        text += QString(" (#%1)").arg(box->order());
    if (port && port->hasValue()) {
        text += "(" + nameOf(port->type()) + ")";
        text += " : " + portValue(port);
    }
    return text + afterEntry();
}

QString BoxOutput::portValue(const Port *port) {
    return isVector(port->type()) ? QString("vector") : port->value<QString>();
}

QString BoxOutput::beforeEntry() {
    QString s;
    switch (_format) {
    case Indented:
        s = indent();
        break;
    case List:
        s = " ";
        break;
    }
    return s;
}

QString BoxOutput::afterEntry() {
    QString s;
    switch (_format) {
    case Indented:
        s = "\n";
        break;
    case List:
        s = " ";
        break;
    }
    return s;
}

QString BoxOutput::indent() {
    QString s;
    for (int i=0; i<_level; ++i)
        s += "  ";
    return s;
}

QString BoxOutput::beginLevel() {
    QString s;
    switch (_format) {
    case Indented:
        ++_level;
        break;
    case List:
        s = "(";
        break;
    }
    return s;
}

QString BoxOutput::endLevel() {
    QString s;
    switch (_format) {
    case Indented:
        --_level;
        break;
    case List:
        s = ")";
        break;
    }
    return s;
}

}
