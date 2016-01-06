#ifndef STRING_CONVERSION
#define STRING_CONVERSION

#include <boost/lexical_cast.hpp>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QTime>
#include "exception.h"
#include "port_type.h"

using boost::lexical_cast;
using boost::bad_lexical_cast;

namespace boxes {

//
// Conversions to string
//

template<class T> QString toString(T source) {
    return QString::number(source);
}
template<> QString toString<char>(char source);
template<> QString toString<bool>(bool source);
template<> QString toString<long double>(long double source);
template<> QString toString<QString>(QString source);
template<> QString toString<QDate>(QDate source);
template<> QString toString<QDateTime>(QDateTime source);
template<> QString toString<QTime>(QTime source);

//
// Conversions from string
//

template<class T> T fromString(QString source) {
    try {
        return lexical_cast<T>(source.toStdString());
    }
    catch(const bad_lexical_cast &)
    {
        throw Exception ("Cannot convert String to " + nameOf(typeOf<T>()), source);
    }
    return T();
}

template<> bool fromString(QString source);
template<> char fromString(QString source);
template<> QString fromString(QString source);
template<> QDate fromString(QString source);
template<> QDateTime fromString(QString source);
template<> QTime fromString(QString source);

} // namespace

#endif

