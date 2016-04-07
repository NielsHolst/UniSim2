#ifndef CONVERT
#define CONVERT

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QTime>
#include "exception.h"
#include "general.h"
#include "port_type.h"

using boost::numeric_cast;
using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::bad_numeric_cast;

namespace base {

//
// Numerical conversions
//

#define NUMERIC_CAST(sourceT) \
try { \
    return numeric_cast<T>(source); \
} \
catch (bad_numeric_cast &ex) { \
    ThrowException("Could not convert " #sourceT " to " + nameOf(typeOf<T>()) + "\n" + ex.what()); \
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
// Numerical conversions to Bool
//
template<> bool convert(char source);
template<> bool convert(int source);
template<> bool convert(long int source);
template<> bool convert(long long int source);
template<> bool convert(float source);
template<> bool convert(double source);
template<> bool convert(long double source);

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
        ThrowException ("Cannot convert String to " + nameOf(typeOf<T>())).value(source);
    }
    return T();
}

template<> bool convert(QString source);
template<> char convert(QString source);
template<> QString convert(QString source);
template<> QStringList convert(QString source);
template<> QDate convert(QString source);
template<> QDateTime convert(QString source);
template<> QTime convert(QString source);

//
// Conversions from String to vector
//

template<> QVector<bool> convert(QString source);
template<> QVector<char> convert(QString source);
template<> QVector<int> convert(QString source);
template<> QVector<long int> convert(QString source);
template<> QVector<long long int> convert(QString source);
template<> QVector<float> convert(QString source);
template<> QVector<double> convert(QString source);
template<> QVector<long double> convert(QString source);
template<> QVector<QString> convert(QString source);
template<> QVector<QDate> convert(QString source);
template<> QVector<QDateTime> convert(QString source);
template<> QVector<QTime> convert(QString source);

//
// Conversions from C string
//

template<class T> T convert(const char *s) { return convert<T>(QString(s)); }

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


//
// Vector conversions
//
#define CANNOT_CONVERT_VECTOR(sourceT) \
ThrowException("Could not convert " #sourceT "Vector to " + nameOf(typeOf<T>()))

template<class T> T convert(QVector<bool>)          { CANNOT_CONVERT_VECTOR(Bool); }
template<class T> T convert(QVector<char>)          { CANNOT_CONVERT_VECTOR(Char); }
template<class T> T convert(QVector<int>)           { CANNOT_CONVERT_VECTOR(Int); }
template<class T> T convert(QVector<long int>)      { CANNOT_CONVERT_VECTOR(LongInt); }
template<class T> T convert(QVector<long long int>) { CANNOT_CONVERT_VECTOR(LongLongInt); }
template<class T> T convert(QVector<float>)         { CANNOT_CONVERT_VECTOR(Float); }
template<class T> T convert(QVector<double>)        { CANNOT_CONVERT_VECTOR(Double); }
template<class T> T convert(QVector<long double>)   { CANNOT_CONVERT_VECTOR(LongDouble); }
template<class T> T convert(QVector<QString>)       { CANNOT_CONVERT_VECTOR(QString); }
template<class T> T convert(QVector<QDate>)         { CANNOT_CONVERT_VECTOR(QDate); }
template<class T> T convert(QVector<QTime>)         { CANNOT_CONVERT_VECTOR(QTime); }
template<class T> T convert(QVector<QDateTime>)     { CANNOT_CONVERT_VECTOR(QDateTime); }

//
// Vector conversions to QStringList
//

template<> QStringList convert(QVector<bool> source);
template<> QStringList convert(QVector<char> source);
template<> QStringList convert(QVector<int> source);
template<> QStringList convert(QVector<long int> source);
template<> QStringList convert(QVector<long long int> source);
template<> QStringList convert(QVector<float> source);
template<> QStringList convert(QVector<double> source);
template<> QStringList convert(QVector<long double> source);
template<> QStringList convert(QVector<QString> source);
template<> QStringList convert(QVector<QDate> source);
template<> QStringList convert(QVector<QTime> source);
template<> QStringList convert(QVector<QDateTime> source);

//
// Vector conversions from QStringList
//

template<class T, template<class> class C> C<T> convert(QStringList list) {
    C<T> result;
    result.reserve(list.size());
    for (QString s : list.toVector())
        result << convert<T>(s);
    return result;
}

//
// Vector conversions to QString
//

template<> QString convert(QVector<bool> source);
template<> QString convert(QVector<char> source);
template<> QString convert(QVector<int> source);
template<> QString convert(QVector<long int> source);
template<> QString convert(QVector<long long int> source);
template<> QString convert(QVector<float> source);
template<> QString convert(QVector<double> source);
template<> QString convert(QVector<long double> source);
template<> QString convert(QVector<QString> source);
template<> QString convert(QVector<QDate> source);
template<> QString convert(QVector<QTime> source);
template<> QString convert(QVector<QDateTime> source);

} // namespace

#endif

