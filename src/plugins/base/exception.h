/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <boost/lexical_cast.hpp>
#include <exception>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QTime>

#define ThrowException(X) \
    throw base::Exception((X)).file(__FILE__).line(__LINE__)

class QObject;

namespace base {
	
class Exception : public std::exception {
public:
    Exception(QString message);
    Exception& file(const char *s);
    Exception& line(int i);
    Exception& context(const QObject *object);
    Exception& hint(QString s);
    const char* what() const _GLIBCXX_USE_NOEXCEPT;
    template <class T> Exception& value(T v);
private:
    QString _message, _value, _hint, _fullName, _file;
    int _line;
};

template <class T> Exception& Exception::value(T v) {
    _value = QString::fromStdString(boost::lexical_cast<std::string>(v));
    return *this;
}

template <> Exception& Exception::value(bool v);
template <> Exception& Exception::value(char v);
template <> Exception& Exception::value(const char *v);
template <> Exception& Exception::value(QString v);
template <> Exception& Exception::value(QDate v);
template <> Exception& Exception::value(QTime v);
template <> Exception& Exception::value(QDateTime v);

}
#endif
