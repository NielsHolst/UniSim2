#include "box.h"
#include "general.h"
#include "port.h"

//
// Macros
//

#define DELETE(X,Y) \
case Y: \
    delete static_cast<X*>(_myData); \
    break; \
case Y##Vector: \
    delete static_cast<Vector##Y*>(_myData); \
    break;

namespace boxes {

//
// Constructor
//

Port::Port(QString name, Box *parent)
    : QObject(parent), _name(name), _myData(0), _type(Null), _parent(parent), _access(Read)
{
    Class(Port);
    setObjectName(name);
    if (parent)
        parent->addPort(this);
}

//
// Destructor
//

Port::~Port()
{
    if (_myData!=0) switch (_type) {
        DELETE(bool, Bool)
        DELETE(char, Char)
        DELETE(int, Int)
        DELETE(long int, LongInt)
        DELETE(long long int, LongLongInt)
        DELETE(float, Float)
        DELETE(double, Double)
        DELETE(long double, LongDouble)
        DELETE(QString, String)
        DELETE(QDate, Date)
        DELETE(QTime, Time)
        DELETE(QDateTime, DateTime)
        case Null: break;
    }
    _myData = 0;
}

//
// Set C string as QString
//

template <> Port& Port::equals<const char*>(const char* x)
{
    equals<QString>(x);
    return *this;
}

//
// Conversions from String
//

template <> bool Port::convert<QString, bool>(QString x) {
    QString s{x.toLower()};
    if (s =="true" || x.toLower()=="t")
        return true;
    else if (s =="false" || x.toLower()=="f")
        return false;
    throw Exception(Port::illegalConversionText<QString,bool>(x));
}
template <> char Port::convert<QString, char>(QString x) {
    if (x.length()!=1)
        throw Exception(Port::illegalConversionText<QString,char>(x));
    return x.at(0).toLatin1();
}
NUMBER_TO_STRING_CONVERT2(int, toInt)
NUMBER_TO_STRING_CONVERT2(long int, toLong)
NUMBER_TO_STRING_CONVERT2(long long int, toLongLong)
NUMBER_TO_STRING_CONVERT2(float, toFloat)
NUMBER_TO_STRING_CONVERT2(double, toDouble)
template <> long double Port::convert<QString, long double>(QString x) {
    long double val;
    try {
        val = boost::lexical_cast<long double>(x.toStdString());
    }
    catch(...) {
        throw Exception(Port::illegalConversionText<QString,long double>(x));
    }
    return val;
}
DATE_TIME_TO_STRING_CONVERT2(QDate,dateFormat)
DATE_TIME_TO_STRING_CONVERT2(QTime,timeFormat)
DATE_TIME_TO_STRING_CONVERT2(QDateTime,dateTimeFormat)
//
// Conversions to Bool
//
NUMBER_TO_BOOL_CONVERT2(char)
NUMBER_TO_BOOL_CONVERT2(int)
NUMBER_TO_BOOL_CONVERT2(long int)
NUMBER_TO_BOOL_CONVERT2(long long int)
NUMBER_TO_BOOL_CONVERT2(float)
NUMBER_TO_BOOL_CONVERT2(double)
NUMBER_TO_BOOL_CONVERT2(long double)
INCOMPATIBLE_CONVERT2(QDate,bool)
INCOMPATIBLE_CONVERT2(QTime,bool)
INCOMPATIBLE_CONVERT2(QDateTime,bool)
//
// Conversions to Char
//
INCOMPATIBLE_CONVERT2(QDate,char)
INCOMPATIBLE_CONVERT2(QTime,char)
INCOMPATIBLE_CONVERT2(QDateTime,char)
//
// Conversions to numbers
//
DATE_TIME_TO_NUMBER_CONVERT2(int)
DATE_TIME_TO_NUMBER_CONVERT2(long int)
DATE_TIME_TO_NUMBER_CONVERT2(long long int)
DATE_TIME_TO_NUMBER_CONVERT2(float)
DATE_TIME_TO_NUMBER_CONVERT2(double)
DATE_TIME_TO_NUMBER_CONVERT2(long double)
//
// Conversions to Date
//
INCOMPATIBLE_CONVERT2(bool, QDate)
INCOMPATIBLE_CONVERT2(char, QDate)
template <> QDate Port::convert<int, QDate>(int x) {
    return QDate(2000,12,31).addDays(x);
}
NUMBER_TO_DATE_CONVERT2(long int)
NUMBER_TO_DATE_CONVERT2(long long int)
NUMBER_TO_DATE_CONVERT2(float)
NUMBER_TO_DATE_CONVERT2(double)
NUMBER_TO_DATE_CONVERT2(long double)
template <> QDate Port::convert<QDate, QDate>(QDate x) {
    return x;
}
INCOMPATIBLE_CONVERT2(QTime,QDate)
template <> QDate Port::convert<QDateTime, QDate>(QDateTime x) {
    return x.date();
}

//
// Conversions to Time
//
INCOMPATIBLE_CONVERT2(bool, QTime)
INCOMPATIBLE_CONVERT2(char, QTime)
template <> QTime Port::convert<int, QTime>(int x) {
    return QTime(0,0).addSecs(x);
}
NUMBER_TO_TIME_CONVERT2(long int)
NUMBER_TO_TIME_CONVERT2(long long int)
NUMBER_TO_TIME_CONVERT2(float)
NUMBER_TO_TIME_CONVERT2(double)
NUMBER_TO_TIME_CONVERT2(long double)
INCOMPATIBLE_CONVERT2(QDate,QTime)
template <> QTime Port::convert<QTime, QTime>(QTime x) {
    return x;
}
template <> QTime Port::convert<QDateTime, QTime>(QDateTime x) {
    return x.time();
}
//
// Conversions to DateTime
//
INCOMPATIBLE_CONVERT2(bool, QDateTime)
INCOMPATIBLE_CONVERT2(char, QDateTime)
template <> QDateTime Port::convert<int, QDateTime>(int x) {
    return QDateTime(QDate(2000,12,31)).addDays(x);
}
NUMBER_TO_DATE_TIME_CONVERT2(long int)
NUMBER_TO_DATE_TIME_CONVERT2(long long int)
NUMBER_TO_DATE_TIME_CONVERT2(float)
NUMBER_TO_DATE_TIME_CONVERT2(double)
NUMBER_TO_DATE_TIME_CONVERT2(long double)
template <> QDateTime Port::convert<QDate, QDateTime>(QDate x) {
    return QDateTime(x);
}
INCOMPATIBLE_CONVERT2(QTime,QDateTime)
template <> QDateTime Port::convert<QDateTime, QDateTime>(QDateTime x) {
    return x;
}
//
// Conversions to String
//
template <> QString Port::convert<bool, QString>(bool x) {
    return x ? "true" : "false";
}
template <> QString Port::convert<char, QString>(char x) {
    return QString(x);
}
NUMBER_TO_STRING2(int)
NUMBER_TO_STRING2(long int)
NUMBER_TO_STRING2(long long int)
NUMBER_TO_STRING2(float)
NUMBER_TO_STRING2(double)
template <> QString Port::convert<long double, QString>(long double x) {
    return QString::fromStdString( boost::lexical_cast<std::string>(x) );
}
template <> QString Port::convert<QDate, QString>(QDate x) {
    return x.toString(dateFormat);
}
template <> QString Port::convert<QTime, QString>(QTime x) {
    return x.toString(timeFormat);
}
template <> QString Port::convert<QDateTime, QString>(QDateTime x) {
    return x.toString(dateTimeFormat);
}
template <> QString Port::convert<QString, QString>(QString x) {
    return x;
}

TEMPLATE_VALUE2(bool,Bool)
TEMPLATE_VALUE2(char,Char)
TEMPLATE_VALUE2(int,Int)
TEMPLATE_VALUE2(long int,LongInt)
TEMPLATE_VALUE2(long long int,LongLongInt)
TEMPLATE_VALUE2(float,Float)
TEMPLATE_VALUE2(double,Double)
TEMPLATE_VALUE2(long double,LongDouble)
TEMPLATE_VALUE2(QString,String)
TEMPLATE_VALUE2(QDate,Date)
TEMPLATE_VALUE2(QTime,Time)
TEMPLATE_VALUE2(QDateTime,Time)

template <> VectorBool Port::value<VectorBool>() const
{
    Q_ASSERT(_data);
    VectorBool val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR_SELF(bool, Bool)
    CASE_CONVERT_TO_VECTOR(char, Char, bool)
    CASE_CONVERT_TO_VECTOR(int, Int, bool)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, bool)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, bool)
    CASE_CONVERT_TO_VECTOR(float, Float, bool)
    CASE_CONVERT_TO_VECTOR(double, Double, bool)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, bool)
    CASE_CONVERT_TO_VECTOR(QString, String, bool)
    CASE_CONVERT_TO_VECTOR(QDate, Date, bool)
    CASE_CONVERT_TO_VECTOR(QTime,Time, bool)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, bool)
    case Null:
        throw conversionError(BoolVector);
    }
    return val;
}

template <> VectorChar Port::value<VectorChar>() const
{
    Q_ASSERT(_data);
    VectorChar val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, char)
    CASE_CONVERT_TO_VECTOR_SELF(char, Char)
    CASE_CONVERT_TO_VECTOR(int, Int, char)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, char)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, char)
    CASE_CONVERT_TO_VECTOR(float, Float, char)
    CASE_CONVERT_TO_VECTOR(double, Double, char)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, char)
    CASE_CONVERT_TO_VECTOR(QString, String, char)
    CASE_CONVERT_TO_VECTOR(QDate, Date, char)
    CASE_CONVERT_TO_VECTOR(QTime,Time, char)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, char)
    case Null:
        throw conversionError(CharVector);
    }
    return val;
}

template <> VectorInt Port::value<VectorInt>() const
{
    Q_ASSERT(_data);
    VectorInt val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, int)
    CASE_CONVERT_TO_VECTOR(char, Char, int)
    CASE_CONVERT_TO_VECTOR_SELF(int, Int)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, int)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, int)
    CASE_CONVERT_TO_VECTOR(float, Float, int)
    CASE_CONVERT_TO_VECTOR(double, Double, int)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, int)
    CASE_CONVERT_TO_VECTOR(QString, String, int)
    CASE_CONVERT_TO_VECTOR(QDate, Date, int)
    CASE_CONVERT_TO_VECTOR(QTime,Time, int)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, int)
    case Null:
        throw conversionError(IntVector);
    }
    return val;
}

template <> VectorLongInt Port::value<VectorLongInt>() const
{
    Q_ASSERT(_data);
    VectorLongInt val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, long int)
    CASE_CONVERT_TO_VECTOR(char, Char, long int)
    CASE_CONVERT_TO_VECTOR(int, Int, long int)
    CASE_CONVERT_TO_VECTOR_SELF(long int, LongInt)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, long int)
    CASE_CONVERT_TO_VECTOR(float, Float, long int)
    CASE_CONVERT_TO_VECTOR(double, Double, long int)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, long int)
    CASE_CONVERT_TO_VECTOR(QString, String, long int)
    CASE_CONVERT_TO_VECTOR(QDate, Date, long int)
    CASE_CONVERT_TO_VECTOR(QTime,Time, long int)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, long int)
    case Null:
        throw conversionError(LongIntVector);
    }
    return val;
}

template <> VectorLongLongInt Port::value<VectorLongLongInt>() const
{
    Q_ASSERT(_data);
    VectorLongLongInt val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, long long int)
    CASE_CONVERT_TO_VECTOR(char, Char, long long int)
    CASE_CONVERT_TO_VECTOR(int, Int, long long int)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, long long int)
    CASE_CONVERT_TO_VECTOR_SELF(long long int, LongLongInt)
    CASE_CONVERT_TO_VECTOR(float, Float, long long int)
    CASE_CONVERT_TO_VECTOR(double, Double, long long int)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, long long int)
    CASE_CONVERT_TO_VECTOR(QString, String, long long int)
    CASE_CONVERT_TO_VECTOR(QDate, Date, long long int)
    CASE_CONVERT_TO_VECTOR(QTime,Time, long long int)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, long long int)
    case Null:
        throw conversionError(LongLongIntVector);
    }
    return val;
}

template <> VectorFloat Port::value<VectorFloat>() const
{
    Q_ASSERT(_data);
    VectorFloat val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, float)
    CASE_CONVERT_TO_VECTOR(char, Char, float)
    CASE_CONVERT_TO_VECTOR(int, Int, float)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, float)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, float)
    CASE_CONVERT_TO_VECTOR_SELF(float, Float)
    CASE_CONVERT_TO_VECTOR(double, Double, float)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, float)
    CASE_CONVERT_TO_VECTOR(QString, String, float)
    CASE_CONVERT_TO_VECTOR(QDate, Date, float)
    CASE_CONVERT_TO_VECTOR(QTime,Time, float)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, float)
    case Null:
        throw conversionError(FloatVector);
    }
    return val;
}

template <> VectorDouble Port::value<VectorDouble>() const
{
    Q_ASSERT(_data);
    VectorDouble val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, double)
    CASE_CONVERT_TO_VECTOR(char, Char, double)
    CASE_CONVERT_TO_VECTOR(int, Int, double)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, double)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, double)
    CASE_CONVERT_TO_VECTOR(float, Float, double)
    CASE_CONVERT_TO_VECTOR_SELF(double, Double)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, double)
    CASE_CONVERT_TO_VECTOR(QString, String, double)
    CASE_CONVERT_TO_VECTOR(QDate, Date, double)
    CASE_CONVERT_TO_VECTOR(QTime,Time, double)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, double)
    case Null:
        throw conversionError(DoubleVector);
    }
    return val;
}

template <> VectorLongDouble Port::value<VectorLongDouble>() const
{
    Q_ASSERT(_data);
    VectorLongDouble val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, long double)
    CASE_CONVERT_TO_VECTOR(char, Char, long double)
    CASE_CONVERT_TO_VECTOR(int, Int, long double)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, long double)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, long double)
    CASE_CONVERT_TO_VECTOR(float, Float, long double)
    CASE_CONVERT_TO_VECTOR(double, Double, long double)
    CASE_CONVERT_TO_VECTOR_SELF(long double, LongDouble)
    CASE_CONVERT_TO_VECTOR(QString, String, long double)
    CASE_CONVERT_TO_VECTOR(QDate, Date, long double)
    CASE_CONVERT_TO_VECTOR(QTime,Time, long double)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, long double)
    case Null:
        throw conversionError(LongDoubleVector);
    }
    return val;
}

template <> VectorString Port::value<VectorString>() const
{
    Q_ASSERT(_data);
    VectorString val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, QString)
    CASE_CONVERT_TO_VECTOR(char, Char, QString)
    CASE_CONVERT_TO_VECTOR(int, Int, QString)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, QString)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, QString)
    CASE_CONVERT_TO_VECTOR(float, Float, QString)
    CASE_CONVERT_TO_VECTOR(double, Double, QString)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, QString)
    CASE_CONVERT_TO_VECTOR_SELF(QString, String)
    CASE_CONVERT_TO_VECTOR(QDate, Date, QString)
    CASE_CONVERT_TO_VECTOR(QTime,Time, QString)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, QString)
     case Null:
        throw conversionError(StringVector);
    }
    return val;
}

template <> VectorDate Port::value<VectorDate>() const
{
    Q_ASSERT(_data);
    VectorDate val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, QDate)
    CASE_CONVERT_TO_VECTOR(char, Char, QDate)
    CASE_CONVERT_TO_VECTOR(int, Int, QDate)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, QDate)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, QDate)
    CASE_CONVERT_TO_VECTOR(float, Float, QDate)
    CASE_CONVERT_TO_VECTOR(double, Double, QDate)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, QDate)
    CASE_CONVERT_TO_VECTOR(QString, String, QDate)
    CASE_CONVERT_TO_VECTOR_SELF(QDate, Date)
    CASE_CONVERT_TO_VECTOR(QTime,Time, QDate)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, QDate)
     case Null:
        throw conversionError(DateVector);
    }
    return val;
}

template <> VectorTime Port::value<VectorTime>() const
{
    Q_ASSERT(_data);
    VectorTime val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, QTime)
    CASE_CONVERT_TO_VECTOR(char, Char, QTime)
    CASE_CONVERT_TO_VECTOR(int, Int, QTime)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, QTime)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, QTime)
    CASE_CONVERT_TO_VECTOR(float, Float, QTime)
    CASE_CONVERT_TO_VECTOR(double, Double, QTime)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, QTime)
    CASE_CONVERT_TO_VECTOR(QString, String, QTime)
    CASE_CONVERT_TO_VECTOR(QDate, Date, QTime)
    CASE_CONVERT_TO_VECTOR_SELF(QTime,Time)
    CASE_CONVERT_TO_VECTOR(QDateTime, DateTime, QTime)
     case Null:
        throw conversionError(TimeVector);
    }
    return val;
}

template <> VectorDateTime Port::value<VectorDateTime>() const
{
    Q_ASSERT(_data);
    VectorDateTime val;
    switch (_type) {
    CASE_CONVERT_TO_VECTOR(bool, Bool, QDateTime)
    CASE_CONVERT_TO_VECTOR(char, Char, QDateTime)
    CASE_CONVERT_TO_VECTOR(int, Int, QDateTime)
    CASE_CONVERT_TO_VECTOR(long int, LongInt, QDateTime)
    CASE_CONVERT_TO_VECTOR(long long int, LongLongInt, QDateTime)
    CASE_CONVERT_TO_VECTOR(float, Float, QDateTime)
    CASE_CONVERT_TO_VECTOR(double, Double, QDateTime)
    CASE_CONVERT_TO_VECTOR(long double, LongDouble, QDateTime)
    CASE_CONVERT_TO_VECTOR(QString, String, QDateTime)
    CASE_CONVERT_TO_VECTOR(QDate, Date, QDateTime)
    CASE_CONVERT_TO_VECTOR(QTime,Time, QDateTime)
    CASE_CONVERT_TO_VECTOR_SELF(QDateTime, DateTime)
     case Null:
        throw conversionError(DateTimeVector);
    }
    return val;
}

}
