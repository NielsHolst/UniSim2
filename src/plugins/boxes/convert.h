#ifndef CONVERT
#define CONVERT

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QTime>
#include "exception.h"
#include "port_type.h"

using boost::numeric_cast;
using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::bad_numeric_cast;

namespace boxes {

//
// Numerical conversions
//

#define NUMERIC_CAST(sourceT) \
try { \
    return numeric_cast<T>(source); \
} \
catch (bad_numeric_cast &ex) { \
    throw Exception("Could not convert " #sourceT " to " + nameOf(typeOf<T>()), ex.what()); \
} \
return T()

template<class T> T convert(bool source)          { NUMERIC_CAST(Bool); }
template<class T> T convert(char source)          { NUMERIC_CAST(Char); }
template<class T> T convert(int source)           { NUMERIC_CAST(Int); }
template<class T> T convert(long int source)      { NUMERIC_CAST(LongInt); }
template<class T> T convert(long long int source) { NUMERIC_CAST(LongLongInt); }
template<class T> T convert(float source)         { NUMERIC_CAST(Float); }
template<class T> T convert(double source)        { NUMERIC_CAST(Double); }
template<class T> T convert(long double source)   { NUMERIC_CAST(LongDouble); }

//
// Numerical conversions to String
//

template<> QString convert(bool source);
template<> QString convert(char source);
template<> QString convert(int source);
template<> QString convert(long int source);
template<> QString convert(long long int source);
template<> QString convert(float source);
template<> QString convert(double source);
template<> QString convert(long double source);

//
// Numerical conversions to Date
//

template<> QDate convert(bool source);
template<> QDate convert(char source);
template<> QDate convert(int source);
template<> QDate convert(long int source);
template<> QDate convert(long long int source);
template<> QDate convert(float source);
template<> QDate convert(double source);
template<> QDate convert(long double source);

//
// Numerical conversions to Time
//

template<> QTime convert(bool source);
template<> QTime convert(char source);
template<> QTime convert(int source);
template<> QTime convert(long int source);
template<> QTime convert(long long int source);
template<> QTime convert(float source);
template<> QTime convert(double source);
template<> QTime convert(long double source);

//
// Numerical conversions to DateTime
//

template<> QDateTime convert(bool source);
template<> QDateTime convert(char source);
template<> QDateTime convert(int source);
template<> QDateTime convert(long int source);
template<> QDateTime convert(long long int source);
template<> QDateTime convert(float source);
template<> QDateTime convert(double source);
template<> QDateTime convert(long double source);


//
// Conversions from String
//

template<class T> T convert(QString source) {
    try {
        return lexical_cast<T>(source.toStdString());
    }
    catch(const bad_lexical_cast &)
    {
        throw Exception ("Cannot convert String to " + nameOf(typeOf<T>()), source);
    }
    return T();
}

template<> bool convert(QString source);
template<> char convert(QString source);
template<> QString convert(QString source);
template<> QDate convert(QString source);
template<> QDateTime convert(QString source);
template<> QTime convert(QString source);

//
// Conversions from QDate
//

template<class T> T convert(QDate source) { return source.dayOfYear(); }
template<> bool convert(QDate);
template<> char convert(QDate);
template<> QString convert(QDate source);
template<> QDate convert(QDate source);
template<> QDateTime convert(QDate source);
template<> QTime convert(QDate);

//
// Conversions from QTime
//

template<class T> T convert(QTime source) {
    double msecs  = source.msecsSinceStartOfDay(),
           hours = msecs/1000./60./60.;
    return numeric_cast<T>(hours);
}
template<> bool convert(QTime);
template<> char convert(QTime);
template<> QString convert(QTime source);
template<> QDate convert(QTime source);
template<> QDateTime convert(QTime source);
template<> QTime convert(QTime);

//
// Conversions from QDateTime
//

template<class T> T convert(QDateTime source) {
    return convert<T>(source.date()) + convert<T>(source.time())/T(24);
}
template<> bool convert(QDateTime);
template<> char convert(QDateTime);
template<> QString convert(QDateTime source);
template<> QDate convert(QDateTime source);
template<> QDateTime convert(QDateTime source);
template<> QTime convert(QDateTime);



} // namespace

#endif

