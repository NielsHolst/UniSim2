/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BASE_EXCEPTION_H
#define BASE_EXCEPTION_H

#include <boost/lexical_cast.hpp>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QTime>
#include "exception_context_class.h"

#define ThrowException(X) \
    throw base::Exception((X)).file(__FILE__).line(__LINE__)

class QObject;

namespace base {
	
class Exception {
public:
    Exception(QString message);
    Exception& file(const char *s);
    Exception& line(int i);
    Exception& context(const QObject *object);
    Exception& hint(QString s);
    Exception& id(QString s);
    template <class T> Exception& value(T v);
    template <class T> Exception& value1(T v);
    template <class T> Exception& value2(T v);

    QString id() const;
    QString what() const;
    static int count();
    static void setContext(const QObject *object);
private:
    // data
    QString _message, _contextDescription,
            _value, _value1, _value2, _hint, _fullName, _file, _id;
    int _line;
    static int _count;
    static const QObject *_fallbackContext;
    // methods
    template <class T> QString asString(T v);
    QDateTime dateTime() const;
};

template <class T> Exception& Exception::value(T v) {
    _value = asString(v);
    return *this;
}

template <class T> Exception& Exception::value1(T v) {
    _value1 = asString(v);
    return *this;
}
template <class T> Exception& Exception::value2(T v) {
    _value2 = asString(v);
    return *this;
}

template <class T> QString Exception::asString(T v) {
    return QString::fromStdString(boost::lexical_cast<std::string>(v));
}

template <> QString Exception::asString(bool v);
template <> QString Exception::asString(char v);
template <> QString Exception::asString(const char *v);
template <> QString Exception::asString(QString v);
template <> QString Exception::asString(QDate v);
template <> QString Exception::asString(QTime v);
template <> QString Exception::asString(QDateTime v);

}
#endif
