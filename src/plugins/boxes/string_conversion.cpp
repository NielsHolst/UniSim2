#include <QStringList>
#include "string_conversion.h"

//
// Conversions to string
//

namespace boxes {

template<> QString toString<char>(char source) {
    return QString(source);
}

template<> QString toString<bool>(bool source) {
    return source ? "TRUE" : "FALSE";
}

template<> QString toString<long double>(long double source) {
    return QString::fromStdString(lexical_cast<std::string>(source));
}

template<> QString toString<QString>(QString source) {
    return source;
}

template<> QString toString<QDate>(QDate source) {
    return source.toString("d/M/yyyy");
}

template<> QString toString<QDateTime>(QDateTime source) {
    return source.toString("d/M/yyyy 0:0:0");
}

template<> QString toString<QTime>(QTime source) {
    return source.toString("0:0:0");
}

//
// Conversions to string
//

template<> bool fromString(QString source) {
    if (source == "T" || source == "TRUE")
        return true;
    else if (source == "F" || source == "FALSE")
        return false;
    else
        throw Exception("Cannot convert String to Bool. Must be T, F, TRUE or FALSE", source);
    return false;
}

template<> char fromString(QString source) {
    if (source.size() == 1)
        return source.at(0).toLatin1();
    else
        throw Exception("Cannot convert String to Char. String must contain exactly one character", source);
    return 0;
}

template<> QString fromString(QString source) {
    return source;
}

template<> QDate fromString(QString source) {
    QString s = source.simplified();
    QDate date = QDate::fromString(s, "d/M/yyyy");
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
        throw Exception("Cannot convert String to Date", source);
    return date;
}

template<> QDateTime fromString(QString source) {
    QString s = source.simplified();
    QStringList parts = s.split(" ");
    QDate date;
    QTime time(0,0,0);
    try {
        date = fromString<QDate>(parts.at(0));
        if (parts.size() > 1)
            time = fromString<QTime>(parts.at(1));
    }
    catch (Exception &) {
    }
    if (!date.isValid() || parts.isEmpty() || parts.size() > 2)
        throw Exception("Cannot convert String to DateTime", source);
    return QDateTime(date, time);
}

template<> QTime fromString(QString source) {
    QString s = source.simplified();
    QTime time = QTime::fromString(s, "h:m:s");
    if (!time.isValid())
        time = QTime::fromString(s, "h:m");
    if (!time.isValid())
        throw Exception("Cannot convert String to Time", source);
    return time;
}

} // namespace
