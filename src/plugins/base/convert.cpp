#include <QStringList>
#include "convert.h"

#define CANNOT_CONVERT(destT, sourceT) \
ThrowException("Cannot convert " #sourceT " to " #destT)

namespace base {

//
// Numerical conversions to Bool
//

template<> bool convert(char source)            { return source!=0; }
template<> bool convert(int source)             { return source!=0; }
template<> bool convert(long int source)        { return source!=0; }
template<> bool convert(long long int source)   { return source!=0; }
template<> bool convert(float source)           { return source!=0; }
template<> bool convert(double source)          { return source!=0; }
template<> bool convert(long double source)     { return source!=0; }

//
// Numerical conversions to String
//

template<> QString convert(bool source)         { return source ? "TRUE" : "FALSE";}
template<> QString convert(char source)         { return QString(source); }
template<> QString convert(int source)          { return QString::number(source); }
template<> QString convert(long int source)     { return QString::number(source); }
template<> QString convert(long long int source){ return QString::number(source); }
template<> QString convert(float source)        { return QString::number(source); }
template<> QString convert(double source)       { return QString::number(source); }
template<> QString convert(long double source)  { return QString::fromStdString(lexical_cast<std::string>(source)); }

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
QTime time = QTime(source, 0, 0); \
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
    if (source == "T" || source == "TRUE")
        return true;
    else if (source == "F" || source == "FALSE")
        return false;
    else
        ThrowException("Cannot convert String to Bool. Must be T, F, TRUE or FALSE").value(source);
    return false;
}

template<> char convert(QString source) {
    if (source.size() == 1)
        return source.at(0).toLatin1();
    else
        ThrowException("Cannot convert String to Char. String must contain exactly one character").value(source);
    return 0;
}

template<> QString convert(QString source) {
    return source;
}


template<> QDate convert(QString source) {
    QString s = source.simplified();
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
    if (!date.isValid() || parts.isEmpty() || parts.size() > 2)
        ThrowException("Cannot convert String to DateTime").value(source);
    return QDateTime(date, time);
}

template<> QTime convert(QString source) {
    QString s = source.simplified();
    QTime time = QTime::fromString(s, "h:m:s");
    if (!time.isValid())
        time = QTime::fromString(s, "h:m");
    if (!time.isValid())
        ThrowException("Cannot convert String to Time").value(source);
    return time;
}

template<> QVector<int> convert(QString source) {
    ThrowException("Here we are: " + source);
}

//
// Conversions from QDate
//

template<> bool convert(QDate)              { ThrowException("Cannot convert Date to Bool"); }
template<> char convert(QDate)              { ThrowException("Cannot convert Date to Char"); }
template<> QString convert(QDate source)    { return source.toString("yyyy/M/d"); }
template<> QDate convert(QDate source)      { return source; }
template<> QDateTime convert(QDate source)  { return QDateTime(source); }
template<> QTime convert(QDate)             { ThrowException("Cannot convert Date to Time"); }

//
// Conversions from QDateTime
//

template<> bool convert(QDateTime)              { ThrowException("Cannot convert DateTime to Bool"); }
template<> char convert(QDateTime)              { ThrowException("Cannot convert DateTime to Char"); }
template<> QString convert(QDateTime source)    { return source.toString("yyyy/M/d hh:mm:ss"); }
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
template<> QStringList convert(QVector<QString> source)         { return QStringList(source.toList()); }
template<> QStringList convert(QVector<QDate> source)           { VECTOR_CONVERT_STRINGLIST(QDate); }
template<> QStringList convert(QVector<QTime> source)           { VECTOR_CONVERT_STRINGLIST(QTime); }
template<> QStringList convert(QVector<QDateTime> source)       { VECTOR_CONVERT_STRINGLIST(QDateTime); }

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
template<> QString convert(QVector<QString> source)         { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<QDate> source)           { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<QTime> source)           { VECTOR_CONVERT_STRING; }
template<> QString convert(QVector<QDateTime> source)       { VECTOR_CONVERT_STRING; }
} // namespace
