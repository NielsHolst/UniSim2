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

Exception::Exception(QString message)
    : _message(message)
{
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
    if (object) {
        _fullName = fullName(object);
        const Box *box = dynamic_cast<const Box *>(object);
        if (!box)
            box = dynamic_cast<const Box *>(object->parent());
        if (box)
            _fullName += QString(" (#%1)").arg(box->order());
    }
    return *this;
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
    QString text = QString{"Error: %1"}.arg(_message);
    if (!_value.isEmpty())
        text += QString("\nValue: '%1'").arg(_value);
    if (!_fullName.isEmpty())
        text += QString("\nObject: %1").arg(_fullName);
    if (!_hint.isEmpty())
        text += "\nHint: " + _hint;
    if (!_file.isEmpty())
        text += QString("\nSource code: %1, line %2").arg(_file).arg(_line);
    return text;
}

template <> Exception& Exception::value(bool v) {
    _value = convert<QString>(v);
    return *this;
}

template <> Exception& Exception::value(char v) {
    _value = convert<QString>(v);
    return *this;
}

template <> Exception& Exception::value(const char *v) {
    _value = convert<QString>(v);
    return *this;
}

template <> Exception& Exception::value(QString v) {
    _value = convert<QString>(v);
    return *this;
}

template <> Exception& Exception::value(QDate v) {
    _value = convert<QString>(v);
    return *this;
}

template <> Exception& Exception::value(QTime v) {
    _value = convert<QString>(v);
    return *this;
}

template <> Exception& Exception::value(QDateTime v) {
    _value = convert<QString>(v);
    return *this;
}


}

