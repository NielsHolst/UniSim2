/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QObject>
#include "exception.h"
#include "general.h"

namespace base {

Exception::Exception(QString message, QString value, const QObject *context, QString hint)
    : _message(message), _value(value), _hint(hint), _context(context)
{
}

QString Exception::message() {
    return _message;
}

QString Exception::value() {
    return _value;
}

const QObject* Exception::context() {
    return _context;
}

const char* Exception::what() const _GLIBCXX_USE_NOEXCEPT {
    QString name = fullName(_context);
    QString msg = _value.isEmpty() ?
                  QString{"Error: '%1'\nObject: '%2'"}.arg(_message).arg(name) :
                  QString{"Error: '%1'\nValue: '%2'\nObject: '%3'"}.arg(_message).arg(_value).arg(name);
    if (!_hint.isEmpty())
        msg += "\nHint: " + _hint;
    return msg.toLocal8Bit();
}

}

