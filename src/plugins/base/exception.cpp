/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <QDateTime>
#include <QObject>
//#include "box.h"
#include "exception.h"
//#include "general.h"
//#include "path.h"

namespace base {

int Exception::_count = 0;
const QObject *Exception::_fallbackContext = nullptr;

Exception::Exception(QString message)
    : _message(message), _caller(Caller())
{
    ++_count;
}

Exception& Exception::file(const char *s) {
    _file = s;
    return *this;
}

Exception& Exception::line(int i) {
    _line = i;
    return *this;
}

Exception& Exception::context(const QObject *) {
    _contextDescription.clear();
//    if (object) {
//        _contextDescription = fullName(object);
//        const Box *box = dynamic_cast<const Box *>(object);
//        if (!box)
//            box = dynamic_cast<const Box *>(object->parent());
//        if (box)
//            _contextDescription += QString(" (#%1)").arg(box->order());
//    }
    return *this;
}

Exception& Exception::caller(Caller c) {
    _caller = c;
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
    QString text = QString{"Error: %1"}.arg(_message),
            description = _contextDescription;
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
//    if (_caller.caller())
//        text += QString("\nCalled by %1\n in %2, line %3").
//                arg(fullName(_caller.caller())).arg(_caller.file()).arg(_caller.line());
//    if (!dateTime().isNull())
//        text += "\nCalendar time: " + convert<QString>(dateTime());
    return text;
}

int Exception::count() {
    return _count;
}

void Exception::setContext(const QObject *object) {
    _fallbackContext = object;
}

template <> QString Exception::asString(bool v) {
    return v ? "true" : "false";
}

template <> QString Exception::asString(char v) {
    return QString(v);
}

template <> QString Exception::asString(const char *v) {
    return QString(v);
}

template <> QString Exception::asString(QString v) {
    return v;
}

template <> QString Exception::asString(QDate v) {
    return v.toString(Qt::ISODate);
}

template <> QString Exception::asString(QTime v) {
    return v.toString("hh:mm:ss");
}

template <> QString Exception::asString(QDateTime v) {
    return v.toString("yyyy/MM/dd hh:mm:ss");
}

QDateTime Exception::dateTime() const {
    QDateTime result;
//    try {
//        Box *calendar = Path("calendar").resolveMaybeOne<Box>();
//        if (calendar) {
//            Port *port = calendar->peakPort("dateTime");
//            if (port) {
//                result = port->value<QDateTime>();
//            }
//        }
//    }
//    catch (...) {
//        result = QDateTime();
//    }
    return result;
}

}

