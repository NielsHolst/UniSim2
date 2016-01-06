#ifndef PORT_H
#define PORT_H
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QVector>
#include "exception.h"
#include "port_type.h"


using boost::numeric_cast;
using std::exception;

namespace boxes {

class Box;

const char dateFormat[] = "dd/MM/yyyy";
const char timeFormat[] = "hh:mm:ss";
const char dateTimeFormat[] = "dd/MM/yyyy hh:mm:ss";


class Port : public QObject {
public:
    enum Access{Read, ReadWrite};
private:
    QString _name;
    void *_value; //, *_myValue;
    const Port *_import;
    PortType _type;
    Box *_parent;
    Access _access;
    bool _doReset;

public:
    Port(QString name="noname", QObject *parent=0);
    QString name() { return _name; }
    Port& access(Access a) { _access = a; return *this; }
    Access access() { return _access; }
    Port& doReset(bool reset) { _doReset = reset; return *this; }
    bool doReset() { return _doReset; }

    template <class T> Port& equals(T )
    {
//        T value(x);
//        PortType xType = typeOf<T>();
//        if (xType != _type) {
//            Port *cached = portCache()->port( typeOf<T>() );
//            cached->equals(x);
//            value = cached->value<T>();
//        };
////        *reinterpret_cast<T*>(_value) = value;
//        assign(value);
//        _import = 0;
        return *this;
    }

    template <class T> void assign(T value)
    {
        throw Exception("Missing assign<T>() implementation for this type", nameOf( typeOf<T>), this);
        return T();
    }

    template <class T> Port& data(T *x)
    {
        _type = typeOf<T>();
        _value = x;
        return *this;
    }

    template <class T> T value() const
    {
        throw Exception("Missing value<T>() implementation for this type", nameOf( typeOf<T>), this);
        return T();
    }

    template <class T> const T* valuePtr() const
    {
        if (_type != typeOf<T>()) {
            QString msg("Port of type %1 cannot be converted to type %2");
            throw Exception(msg.arg(nameOf(_type)).arg(nameOf(typeOf<T>())), "", this);
        }
        return reinterpret_cast<const T*>(_value);
    }

    void import(const Port *source)
    {
        _import = source;
    }

    void doImport();

    void reset();

private:

//    template <class T> void assign(T x);

    Exception conversionError(PortType destination) const {
        QString s{"Cannot convert %1 to %2"};
        return Exception(s.arg(nameOf(_type)).arg(nameOf(destination)));
    }

    template <class T, class U> static U convert(T x) {
        try {
            return numeric_cast<U>(x);
        }
        catch (exception &ex) {
            QString s = convert<T,QString>(x);
            throw Exception(illegalConversionText<T,U>(s));
        }
    }

    template <class T, class U> static QString incompatibleTypesText() {
        QString msg("Incompatible types: cannot convert %1 to %2");
        return msg.arg(nameOf(typeOf<T>())).arg(nameOf(typeOf<U>()));
    }

    template <class T, class U> static QString illegalConversionText(QString value) {
        QString msg("Invalid conversion: cannot convert %1 (value=%2) to %3");
        return msg.arg(nameOf(typeOf<T>())).arg(value).arg(nameOf(typeOf<U>()));
    }

};


//
// SetFixed C-string
//

template <> Port& Port::equals<const char*>(const char* x);

//
// Assignment
//

//template <> void Port::assign<int>(int value) {
//    *reinterpret_cast<int*>(_value) = value;
//}

//
// Conversions from String
//

#define INCOMPATIBLE_CONVERT1(X,Y) \
template <> Y Port::convert<X, Y>(X);

#define INCOMPATIBLE_CONVERT2(X,Y) \
template <> Y Port::convert<X, Y>(X) { \
    throw Exception(Port::incompatibleTypesText<X,Y>()); \
    Y y{}; \
    return y; \
}

#define NUMBER_TO_STRING_CONVERT1(X, Z) \
template <> X Port::convert<QString, X>(QString x);

#define NUMBER_TO_STRING_CONVERT2(X, Z) \
template <> X Port::convert<QString, X>(QString x) { \
    bool ok; \
    X val = x.Z(&ok); \
    if (!ok) \
        throw Exception(Port::illegalConversionText<QString,X>(x)); \
    return val; \
}
#define DATE_TIME_TO_STRING_CONVERT1(X,Z) \
template <> X Port::convert<QString, X>(QString x);

#define DATE_TIME_TO_STRING_CONVERT2(X,Z) \
template <> X Port::convert<QString, X>(QString x) { \
    X val = X::fromString(x, Z); \
    if (!val.isValid()) \
        throw Exception(Port::illegalConversionText<QString,X>(x)); \
    return val; \
}
#define DATE_TIME_TO_NUMBER_CONVERT1(X) \
template <> X Port::convert<QDate, X>(QDate x); \
template <> X Port::convert<QTime, X>(QTime x); \
template <> X Port::convert<QDateTime, X>(QDateTime x);
#define DATE_TIME_TO_NUMBER_CONVERT2(X) \
template <> X Port::convert<QDate, X>(QDate x) { \
    return x.dayOfYear(); \
} \
template <> X Port::convert<QTime, X>(QTime x) { \
    return x.msecsSinceStartOfDay()/1000; \
} \
template <> X Port::convert<QDateTime, X>(QDateTime x) { \
    return x.date().dayOfYear(); \
}

template <> bool Port::convert<QString, bool>(QString x);
template <> char Port::convert<QString, char>(QString x);
NUMBER_TO_STRING_CONVERT1(int, toInt)
NUMBER_TO_STRING_CONVERT1(long int, toLong)
NUMBER_TO_STRING_CONVERT1(long long int, toLongLong)
NUMBER_TO_STRING_CONVERT1(float, toFloat)
NUMBER_TO_STRING_CONVERT1(double, toDouble)
template <> long double Port::convert<QString, long double>(QString x);
DATE_TIME_TO_STRING_CONVERT1(QDate,dateFormat)
DATE_TIME_TO_STRING_CONVERT1(QTime,timeFormat)
DATE_TIME_TO_STRING_CONVERT1(QDateTime,dateTimeFormat)
//
// Conversions to Bool
//
#define NUMBER_TO_BOOL_CONVERT1(X) \
template <> bool Port::convert<X, bool>(X x);
#define NUMBER_TO_BOOL_CONVERT2(X) \
template <> bool Port::convert<X, bool>(X x) { \
    return x!=0; \
}
NUMBER_TO_BOOL_CONVERT1(char)
NUMBER_TO_BOOL_CONVERT1(int)
NUMBER_TO_BOOL_CONVERT1(long int)
NUMBER_TO_BOOL_CONVERT1(long long int)
NUMBER_TO_BOOL_CONVERT1(float)
NUMBER_TO_BOOL_CONVERT1(double)
NUMBER_TO_BOOL_CONVERT1(long double)
INCOMPATIBLE_CONVERT1(QDate,bool)
INCOMPATIBLE_CONVERT1(QTime,bool)
INCOMPATIBLE_CONVERT1(QDateTime,bool)
//
// Conversions to Char
//
INCOMPATIBLE_CONVERT1(QDate,char)
INCOMPATIBLE_CONVERT1(QTime,char)
INCOMPATIBLE_CONVERT1(QDateTime,char)
//
// Conversions to numbers
//
DATE_TIME_TO_NUMBER_CONVERT1(int)
DATE_TIME_TO_NUMBER_CONVERT1(long int)
DATE_TIME_TO_NUMBER_CONVERT1(long long int)
DATE_TIME_TO_NUMBER_CONVERT1(float)
DATE_TIME_TO_NUMBER_CONVERT1(double)
DATE_TIME_TO_NUMBER_CONVERT1(long double)
//
// Conversions to Date
//
#define NUMBER_TO_DATE_CONVERT1(X) \
template <> QDate Port::convert<X, QDate>(X x);
#define NUMBER_TO_DATE_CONVERT2(X) \
template <> QDate Port::convert<X, QDate>(X x) { \
    return convert<int, QDate>(numeric_cast<int>(x)); \
}

INCOMPATIBLE_CONVERT1(bool, QDate)
INCOMPATIBLE_CONVERT1(char, QDate)
template <> QDate Port::convert<int, QDate>(int x);
NUMBER_TO_DATE_CONVERT1(long int)
NUMBER_TO_DATE_CONVERT1(long long int)
NUMBER_TO_DATE_CONVERT1(float)
NUMBER_TO_DATE_CONVERT1(double)
NUMBER_TO_DATE_CONVERT1(long double)
template <> QDate Port::convert<QDate, QDate>(QDate x);
INCOMPATIBLE_CONVERT1(QTime,QDate)
template <> QDate Port::convert<QDateTime, QDate>(QDateTime x);
//
// Conversions to Time
//
#define NUMBER_TO_TIME_CONVERT1(X) \
template <> QTime Port::convert<X, QTime>(X x);
#define NUMBER_TO_TIME_CONVERT2(X) \
template <> QTime Port::convert<X, QTime>(X x) { \
    return convert<int, QTime>(numeric_cast<int>(x)); \
}
INCOMPATIBLE_CONVERT1(bool, QTime)
INCOMPATIBLE_CONVERT1(char, QTime)
template <> QTime Port::convert<int, QTime>(int x);
NUMBER_TO_TIME_CONVERT1(long int)
NUMBER_TO_TIME_CONVERT1(long long int)
NUMBER_TO_TIME_CONVERT1(float)
NUMBER_TO_TIME_CONVERT1(double)
NUMBER_TO_TIME_CONVERT1(long double)
INCOMPATIBLE_CONVERT1(QDate,QTime)
template <> QTime Port::convert<QTime, QTime>(QTime x);
template <> QTime Port::convert<QDateTime, QTime>(QDateTime x);
//
// Conversions to DateTime
//
#define NUMBER_TO_DATE_TIME_CONVERT1(X) \
template <> QDateTime Port::convert<X, QDateTime>(X x);
#define NUMBER_TO_DATE_TIME_CONVERT2(X) \
template <> QDateTime Port::convert<X, QDateTime>(X x) { \
    return convert<int, QDateTime>(numeric_cast<int>(x)); \
}

INCOMPATIBLE_CONVERT1(bool, QDateTime)
INCOMPATIBLE_CONVERT1(char, QDateTime)
template <> QDateTime Port::convert<int, QDateTime>(int x);
NUMBER_TO_DATE_TIME_CONVERT1(long int)
NUMBER_TO_DATE_TIME_CONVERT1(long long int)
NUMBER_TO_DATE_TIME_CONVERT1(float)
NUMBER_TO_DATE_TIME_CONVERT1(double)
NUMBER_TO_DATE_TIME_CONVERT1(long double)
template <> QDateTime Port::convert<QDate, QDateTime>(QDate x);
INCOMPATIBLE_CONVERT1(QTime,QDateTime)
template <> QDateTime Port::convert<QDateTime, QDateTime>(QDateTime x);

//
// Conversions to String
//
#define NUMBER_TO_STRING1(X) \
template <> QString Port::convert<X, QString>(X x);
#define NUMBER_TO_STRING2(X) \
template <> QString Port::convert<X, QString>(X x) { \
    return QString::number(x); \
}

template <> QString Port::convert<bool, QString>(bool x);
template <> QString Port::convert<char, QString>(char x);
NUMBER_TO_STRING1(int)
NUMBER_TO_STRING1(long int)
NUMBER_TO_STRING1(long long int)
NUMBER_TO_STRING1(float)
NUMBER_TO_STRING1(double)
template <> QString Port::convert<long double, QString>(long double x);
template <> QString Port::convert<QDate, QString>(QDate x);
template <> QString Port::convert<QTime, QString>(QTime x);
template <> QString Port::convert<QDateTime, QString>(QDateTime x);
template <> QString Port::convert<QString, QString>(QString x);

//
// Value (scalar)
//

#define CASE_CONVERT(X,Y,Z) \
case Y: \
    return Port::convert<X,Z>(*static_cast<const X*>(_value)); \
case Y##Vector: \
    p.vec##Y = static_cast<const Vector##Y*>(_value); \
    if (p.vec##Y->length()!=1) { \
        QString s{"%1 (length=%2) must have length one to be used as a scalar"}; \
        throw Exception(s.arg(nameOf(Y##Vector)).arg(p.vec##Y->length())); \
    } \
    return Port::convert<X,Z>(p.vec##Y->at(0));

#define CASE_CONVERT_TO_VECTOR(X,Y,Z) \
case Y: \
    return QVector<Z>(1, value<Z>()); \
case Y##Vector: \
    for (auto v : *static_cast<const Vector##Y*>(_value)) \
        val << Port::convert<X,Z>(v); \
    break;

#define CASE_CONVERT_TO_VECTOR_SELF(X,Y) \
case Y: \
    return QVector<X>(1, *static_cast<const X*>(_value)); \
case Y##Vector: \
    return *static_cast<const QVector<X>*>(_value);

#define VECTOR_PTR(X,Y) const QVector<X> *vec##Y;

union VectorPointer {
    VECTOR_PTR(bool, Bool)
    VECTOR_PTR(char, Char)
    VECTOR_PTR(int, Int)
    VECTOR_PTR(long int, LongInt)
    VECTOR_PTR(long long int, LongLongInt)
    VECTOR_PTR(float, Float)
    VECTOR_PTR(double, Double)
    VECTOR_PTR(long double, LongDouble)
    VECTOR_PTR(QString, String)
    VECTOR_PTR(QDate, Date)
    VECTOR_PTR(QTime, Time)
    VECTOR_PTR(QDateTime, DateTime)
};

#define TEMPLATE_VALUE1(X,Y) \
template <> X Port::value<X>() const;
#define TEMPLATE_VALUE2(X,Y) \
template <> X Port::value<X>() const \
{ \
    Q_ASSERT(_value); \
    VectorPointer p; \
    switch (_type) { \
    CASE_CONVERT(bool, Bool, X) \
    CASE_CONVERT(char, Char, X) \
    CASE_CONVERT(int, Int, X) \
    CASE_CONVERT(long int, LongInt, X) \
    CASE_CONVERT(long long int, LongLongInt, X) \
    CASE_CONVERT(float, Float, X) \
    CASE_CONVERT(double, Double, X) \
    CASE_CONVERT(long double, LongDouble, X) \
    CASE_CONVERT(QString, String, X) \
    CASE_CONVERT(QDate, Date, X) \
    CASE_CONVERT(QTime,Time, X) \
    CASE_CONVERT(QDateTime, DateTime, X) \
    case Null: \
        throw conversionError(Y); \
    } \
    X x{}; \
    return x; \
}

TEMPLATE_VALUE1(bool,Bool)
TEMPLATE_VALUE1(char,Char)
TEMPLATE_VALUE1(int,Int)
TEMPLATE_VALUE1(long int,LongInt)
TEMPLATE_VALUE1(long long int,LongLongInt)
TEMPLATE_VALUE1(float,Float)
TEMPLATE_VALUE1(double,Double)
TEMPLATE_VALUE1(long double,LongDouble)
TEMPLATE_VALUE1(QString,String)
TEMPLATE_VALUE1(QDate,Date)
TEMPLATE_VALUE1(QTime,Time)
TEMPLATE_VALUE1(QDateTime,Time)

//
// VECTOR values
//

template <> VectorBool Port::value<VectorBool>() const;
template <> VectorChar Port::value<VectorChar>() const;
template <> VectorInt Port::value<VectorInt>() const;
template <> VectorLongInt Port::value<VectorLongInt>() const;
template <> VectorLongLongInt Port::value<VectorLongLongInt>() const;
template <> VectorFloat Port::value<VectorFloat>() const;
template <> VectorDouble Port::value<VectorDouble>() const;
template <> VectorLongDouble Port::value<VectorLongDouble>() const;
template <> VectorString Port::value<VectorString>() const;
template <> VectorDate Port::value<VectorDate>() const;
template <> VectorTime Port::value<VectorTime>() const;
template <> VectorDateTime Port::value<VectorDateTime>() const;

}
#endif
