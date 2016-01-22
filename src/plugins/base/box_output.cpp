#include <iostream>
#include "box.h"
#include "box_output.h"
#include "exception.h"
#include "general.h"

namespace base {

BoxOutput::BoxOutput(const Box *box, Format format)
    : _box(box), _format(format), _level(0)
{
}

QString BoxOutput::asText() {
    _output = "";
    addOutput(_box);
    return _output;
}

void BoxOutput::addOutput(const QObject *box) {
    _output += name(box);
    auto children = box->findChildren<const QObject*>(QString(), Qt::FindDirectChildrenOnly);
    if (!children.isEmpty()) {
        _output += beginLevel();
        for (const QObject *child : children) {
//            auto box = dynamic_cast<const Box*>(child);
//            if (box) addOutput(box);
            addOutput(child);
        }
        _output += endLevel();
    }
}

QString BoxOutput::name(const QObject *box) {
    return beforeName() + className(box) + " " + box->objectName() + afterName();
}

QString BoxOutput::beforeName() {
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

QString BoxOutput::afterName() {
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
        s += "\t";
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
