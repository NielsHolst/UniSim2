/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QLocale>
#include <QStringList>
#include <QTimeZone>
#include "any_year.h"
#include "convert.h"

#define CANNOT_CONVERT(destT, sourceT) \
ThrowException("Cannot convert " #sourceT " to " #destT)

namespace base {

namespace {
    QString _localeName;
    QChar _outputDecimalCharacter = '.';
    QLocale _locale;
}

void setLocale(QString localeName) {
    _localeName = localeName.toLower();
    #if QT_VERSION < 0x060000
        _outputDecimalCharacter = (_localeName=="local") ? QLocale().decimalPoint() : QChar('.');
    #else
        _outputDecimalCharacter = (_localeName=="local") ? QLocale().decimalPoint().at(0) : QChar('.');
    #endif
    _locale = QLocale();
    _locale.setNumberOptions(QLocale::OmitGroupSeparator);
}

QString localeName() {
    return _localeName;
}

QChar outputDecimalCharacter() {
    return _outputDecimalCharacter;
}

//
// Numerical conversions to Bool
//

template<> bool convert(char source)            { return source!=0; }
template<> bool convert(int source)             { return source!=0; }
template<> bool convert(long int source)        { return source!=0; }
template<> bool convert(long long int source)   { return source!=0; }
template<> bool convert(float source)           { return static_cast<int>(source)!=0; }
template<> bool convert(double source)          { return static_cast<int>(source)!=0; }
template<> bool convert(long double source)     { return static_cast<int>(source)!=0; }

//
// Numerical conversions to String
//

static QString withDecimal(QString s) {
    return s.startsWith("nan") ? "NA" :
           (s.contains(outputDecimalCharacter()) || s.contains("e") || s.contains("inf")) ? s : s + outputDecimalCharacter() + "0";
}

template<> QString convert(bool source) {
    return source ? "TRUE" : "FALSE";
}
template<> QString convert(char source) {
    return QString(source);
}
template<> QString convert(int source) {
    return QString::number(source);
}
template<> QString convert(long int source) {
    return QString::number(source);
}
template<> QString convert(long long int source){
    return QString::number(source);
}
template<> QString convert(float source)  {
    return withDecimal(
                _localeName.isEmpty() ? QString::number(static_cast<double>(source)) : _locale.toString(static_cast<double>(source))
           );
}
template<> QString convert(double source) {
    return withDecimal(
                _localeName.isEmpty() ? QString::number(source) : _locale.toString(source)
           );
}
template<> QString convert(long double source) {
    return withDecimal(
                QString::fromStdString(lexical_cast<std::string>(source))
           );
}

//
// Numerical conversions to Date
//

template<> QDate convert(bool)          { CANNOT_CONVERT(Date, Bool); }
template<> QDate convert(char)          { CANNOT_CONVERT(Date, Char); }
template<> QDate convert(int)           { CANNOT_CONVERT(Date, Int); }
template<> QDate convert(long int)      { CANNOT_CONVERT(Date, LongInt); }
template<> QDate convert(long long int) { CANNOT_CONVERT(Date, LongLongInt); }
template<> QDate convert(float)         { CANNOT_CONVERT(Date, Float); }
template<> QDate convert(double)        { CANNOT_CONVERT(Date, Double); }
template<> QDate convert(long double)   { CANNOT_CONVERT(Date, LongDouble); }

//
// Numerical conversions to Time
//

#define TIME_CONVERT(sourceT) \
QTime time = QTime(static_cast<int>(source), 0, 0); \
if (!time.isValid()) \
    CANNOT_CONVERT(QTime, sourceT); \
return time

template<> QTime convert(bool)                  { CANNOT_CONVERT(Time, Bool); }
template<> QTime convert(char source)           { TIME_CONVERT(Char); }
template<> QTime convert(int source)            { TIME_CONVERT(Int); }
template<> QTime convert(long int source)       { TIME_CONVERT(LongInt); }
template<> QTime convert(long long int source)  { TIME_CONVERT(LongLongInt); }
template<> QTime convert(float source)          { TIME_CONVERT(Float); }
template<> QTime convert(double source)         { TIME_CONVERT(Double); }
template<> QTime convert(long double source)    { TIME_CONVERT(LongDouble); }

//
// Numerical conversions to DateTime
//

template<> QDateTime convert(bool)          { CANNOT_CONVERT(DateTime, Bool); }
template<> QDateTime convert(char)          { CANNOT_CONVERT(DateTime, Char); }
template<> QDateTime convert(int)           { CANNOT_CONVERT(DateTime, Int); }
template<> QDateTime convert(long int)      { CANNOT_CONVERT(DateTime, LongInt); }
template<> QDateTime convert(long long int) { CANNOT_CONVERT(DateTime, LongLongInt); }
template<> QDateTime convert(float)         { CANNOT_CONVERT(DateTime, Float); }
template<> QDateTime convert(double)        { CANNOT_CONVERT(DateTime, Double); }
template<> QDateTime convert(long double)   { CANNOT_CONVERT(DateTime, LongDouble); }

//
// Conversions from String
//

template<> bool convert(QString source) {
    if (source == "1" || source == "T" || source == "TRUE")
        return true;
    else if (source == "0" || source == "F" || source == "FALSE")
        return false;
    else
        ThrowException("Cannot convert String to Bool. Must be 0, 1, T, F, TRUE or FALSE").value(source);
}

template<> char convert(QString source) {
    if (source.size() == 1)
        return source.at(0).toLatin1();
    else
        ThrowException("Cannot convert String to Char. String must contain exactly one character").value(source);
}

template<> QString convert(QString source) {
    return source;
}

template<> QStringList convert(QString source) {
    return split(source);
}


template<> QDate convert(QString source) {
    QString s = source.trimmed();
    bool hasAnyYear = (s.startsWith("*") != s.endsWith("*"));  // xor
    if (hasAnyYear)
        s.replace("*", "2000");
    QDate date = QDate::fromString(s, "d/M/yyyy");
    if (!date.isValid())
        date = QDate::fromString(s, "/M/d/yyyy");
    if (!date.isValid())
        date = QDate::fromString(s, "/M/d/yy");
    if (!date.isValid())
        date = QDate::fromString(s, "d.M.yyyy");
    if (!date.isValid())
        date = QDate::fromString(s, "d-M-yyyy");
    if (!date.isValid())
        date = QDate::fromString(s, "yyyy/M/d");
    if (!date.isValid())
        date = QDate::fromString(s, "yyyy.M.d");
    if (!date.isValid())
        date = QDate::fromString(s, "yyyy-M-d");
    if (!date.isValid())
        ThrowException("Cannot convert String to Date").value(source);
    if (hasAnyYear)
        setAnyYear(date);
    return date;
}

template<> QDateTime convert(QString source) {
    QString s = source.simplified();
    QStringList parts = s.split(" ");
    QDate date;
    QTime time(0,0,0);
    try {
        date = convert<QDate>(parts.at(0));
        if (parts.size() > 1)
            time = convert<QTime>(parts.at(1));
    }
    catch (Exception &) {
    }

//    if (source.contains("*<Calendar>[dateTime]")) {
//        if (!date.isValid() || parts.isEmpty() || parts.size() > 2)
//            ThrowException("Cannot convert String to DateTime").value(source);
//    }
    if (!date.isValid() || parts.isEmpty() || parts.size() > 2)
        ThrowException("Cannot convert String to DateTime").value(source);

    return QDateTime(date, time);
}

template<> QTime convert(QString source) {
    QString s = source.trimmed();
    QTime time = QTime::fromString(s, "h:m:s");
    if (!time.isValid())
        time = QTime::fromString(s, "h:m");
    if (!time.isValid())
        ThrowException("Cannot convert String to Time").value(source);
    return time;
}

//
// Conversions from String to vector
//

#define CONVERT_STRING_TO_VECTOR(X) \
template<> QVector<X> convert(QString source) { \
    QStringList list = split(source); \
    return convert<X, QVector>(list); \
}

//template<> QVector<bool> convert(QString source) {
//    QStringList list = split(source);
//    return convert<bool, QVector>(list  );
//}

CONVERT_STRING_TO_VECTOR(bool)
CONVERT_STRING_TO_VECTOR(char)
CONVERT_STRING_TO_VECTOR(int)
CONVERT_STRING_TO_VECTOR(long int)
CONVERT_STRING_TO_VECTOR(long long int)
CONVERT_STRING_TO_VECTOR(float)
CONVERT_STRING_TO_VECTOR(double)
CONVERT_STRING_TO_VECTOR(long double)
CONVERT_STRING_TO_VECTOR(QDate)
CONVERT_STRING_TO_VECTOR(QDateTime)
CONVERT_STRING_TO_VECTOR(QTime)

//template<> QVector<QString> convert(QString source) {
//    return split(source).toVector();
//}

//
// Conversions from QDate
//

template<> bool convert(QDate)              { ThrowException("Cannot convert Date to Bool"); }
template<> char convert(QDate)              { ThrowException("Cannot convert Date to Char"); }
template<> QString convert(QDate source) {
    QString s;
    if (hasAnyYear(source)) {
        s = "*/%1/%2";
        s = s.arg(source.month()).arg(source.day());
    }
    else {
        s = source.toString("yyyy/M/d");
    }
    return s;
}
template<> QDate convert(QDate source)      { return source; }
template<> QDateTime convert(QDate source)  { return QDateTime(source, QTime(), QTimeZone(0)); }
template<> QTime convert(QDate)             { ThrowException("Cannot convert Date to Time"); }

//
// Conversions from QDateTime
//

template<> bool convert(QDateTime)              { ThrowException("Cannot convert DateTime to Bool"); }
template<> char convert(QDateTime)              { ThrowException("Cannot convert DateTime to Char"); }
template<> QString convert(QDateTime source)    { return source.toString("yyyy/M/d hh:mm:ss");}
template<> QDate convert(QDateTime source)      { return source.date(); }
template<> QDateTime convert(QDateTime source)  { return source; }
template<> QTime convert(QDateTime source)      { return source.time(); }

//
// Conversions from QTime
//

template<> bool convert(QTime)              { ThrowException("Cannot convert Time to Bool"); }
template<> char convert(QTime)              { ThrowException("Cannot convert Time to Char"); }
template<> QString convert(QTime source)    { return source.toString("hh:mm:ss"); }
template<> QDate convert(QTime)             { ThrowException("Cannot convert Time to Date"); }
template<> QDateTime convert(QTime)         { ThrowException("Cannot convert Time to DateTime"); }
template<> QTime convert(QTime source)      { return source; }

//
// Vector conversions to QStringList
//

#define VECTOR_CONVERT_STRINGLIST(sourceT) \
QStringList s; \
for (sourceT v : source) s << convert<QString>(v); \
return s

template<> QStringList convert(QVector<bool> source)            { VECTOR_CONVERT_STRINGLIST(bool); }
template<> QStringList convert(QVector<char> source)            { VECTOR_CONVERT_STRINGLIST(char); }
template<> QStringList convert(QVector<int> source)             { VECTOR_CONVERT_STRINGLIST(int); }
template<> QStringList convert(QVector<long int> source)        { VECTOR_CONVERT_STRINGLIST(long int); }
template<> QStringList convert(QVector<long long int> source)   { VECTOR_CONVERT_STRINGLIST(long long int); }
template<> QStringList convert(QVector<float> source)           { VECTOR_CONVERT_STRINGLIST(float); }
template<> QStringList convert(QVector<double> source)          { VECTOR_CONVERT_STRINGLIST(double); }
template<> QStringList convert(QVector<long double> source)     { VECTOR_CONVERT_STRINGLIST(long double); }
template<> QStringList convert(QVector<QString> source)         {
    return QStringList(QList<QString>(source.begin(), source.end()));
}
template<> QStringList convert(QVector<QDate> source)           { VECTOR_CONVERT_STRINGLIST(QDate); }
template<> QStringList convert(QVector<QTime> source)           { VECTOR_CONVERT_STRINGLIST(QTime); }
template<> QStringList convert(QVector<QDateTime> source)       { VECTOR_CONVERT_STRINGLIST(QDateTime); }

//
// Vector conversions from QStringList
//  (defined in header file)
//

//
// Vector conversions to QString
//

#define VECTOR_CONVERT_STRING \
return "(" + convert<QStringList>(source).join(" ") + ")"

template<> QString convert(QVector<bool> source)            { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<char> source)            { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<int> source)             { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<long int> source)        { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<long long int> source)   { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<float> source)           { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<double> source)          { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<long double> source)     { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<QString> source)         {
    QStringList list = QStringList( QList<QString>(source.begin(), source.end()) );
    return "(" + list.join(" ") + ")";
}
template<> QString convert(QVector<QDate> source)           { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<QTime> source)           { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<QDateTime> source)       { VECTOR_CONVERT_STRING; }

} // namespace
