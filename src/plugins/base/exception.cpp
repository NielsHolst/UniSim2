/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QObject>
#include "box.h"
#include "convert.h"
#include "exception.h"
#include "general.h"

namespace base {

int Exception::_count = 0;
const QObject *Exception::_fallbackContext = nullptr;

Exception::Exception(QString message)
    : _context(_fallbackContext), _message(message)
{
    ++_count;
}

Exception::~Exception() {
    _fallbackContext = nullptr;
}

Exception& Exception::file(const char *s) {
    _file = s;
    return *this;
}

Exception& Exception::line(int i) {
    _line = i;
    return *this;
}

Exception& Exception::context(const QObject *object) {
    _context = object;
    return *this;
}

QString Exception::contextDescription() const {
    QString description;
    const QObject *context = _context ? _context : _fallbackContext;
    if (context) {
        description = fullName(context);
        const Box *box = dynamic_cast<const Box *>(context);
        if (!box)
            box = dynamic_cast<const Box *>(context->parent());
        if (box)
            description += QString(" (#%1)").arg(box->order());
    }
    return description;
}

Exception& Exception::hint(QString s) {
    _hint = s;
    return *this;
}

Exception& Exception::id(QString s) {
    _id = s;
    return *this;
}

QString Exception::id() const {
    return _id;
}

QString Exception::what() const {
    QString text = QString{"Error: %1"}.arg(_message),
            description = contextDescription();
    if (!_value.isNull())
        text += QString("\nValue: '%1'").arg(_value);
    if (!_value1.isNull())
        text += QString("\nValue1: '%1'").arg(_value1);
    if (!_value2.isNull())
        text += QString("\nValue2: '%1'").arg(_value2);
    if (!description.isEmpty())
        text += QString("\nObject: %1").arg(description);
    if (!_hint.isEmpty())
        text += "\nHint: " + _hint;
    if (!_file.isEmpty())
        text += QString("\nSource code: %1, line %2").arg(_file).arg(_line);
    return text;
}

int Exception::count() {
    return _count;
}

void Exception::setContext(const QObject *object) {
    _fallbackContext = object;
}

template <> QString Exception::asString(bool v) {
    return convert<QString>(v);
}

template <> QString Exception::asString(char v) {
    return convert<QString>(v);
}

template <> QString Exception::asString(const char *v) {
    return convert<QString>(v);
}

template <> QString Exception::asString(QString v) {
    return convert<QString>(v);
}

template <> QString Exception::asString(QDate v) {
    return convert<QString>(v);
}

template <> QString Exception::asString(QTime v) {
    return convert<QString>(v);
}

template <> QString Exception::asString(QDateTime v) {
    return convert<QString>(v);
}


}

