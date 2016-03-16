/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QObject>
#include "convert.h"
#include "exception.h"
#include "general.h"

namespace base {

Exception::Exception(QString message)
    : std::exception(), _message(message)
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
    _fullName = object ? fullName(object) : "";
    return *this;
}

Exception& Exception::hint(QString s) {
    _hint = s;
    return *this;
}

const char* Exception::what() const _GLIBCXX_USE_NOEXCEPT {
    QString text = QString{"Error: %1"}.arg(_message);
    if (!_value.isEmpty())
        text += QString("\nValue: '%1'").arg(_value);
    if (!_fullName.isEmpty())
        text += QString("\nObject: %1").arg(_fullName);
    if (!_hint.isEmpty())
        text += "\nHint: " + _hint;
    if (!_file.isEmpty())
        text += QString("\nSource code: %1, line %2").arg(_file).arg(_line);
    return text.toLocal8Bit();
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

