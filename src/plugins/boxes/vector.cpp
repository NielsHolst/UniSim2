#include <QStringList>
#include <QVector>
#include "convert.h"
#include "port.h"
#include "vector.h"

#define VALUE(T) \
    (*(reinterpret_cast<const T*>(valuePtr)))

#define VECTOR_PTR(T) \
    (reinterpret_cast<QVector<T>*>(_vectorPtr))

#define CASE_RESERVE(Tcpp, Tport, size) \
    case Tport: \
    _vectorPtr = new QVector<Tcpp>; \
    VECTOR_PTR(Tcpp)->reserve(size); \
    break; \
    case Tport##Vector: \
    _vectorPtr = new QVector<QVector<Tcpp>>; \
    VECTOR_PTR(QVector<Tcpp>)->reserve(size); \
    break;

#define CASE_ERASE(Tcpp, Tport) \
    case Tport: \
    delete VECTOR_PTR(Tcpp); \
    break; \
    case Tport##Vector: \
    delete VECTOR_PTR(QVector<Tcpp>); \
    break;

#define CASE_APPEND(Tcpp, Tport) \
    case Tport: \
    VECTOR_PTR(Tcpp)->append(VALUE(Tcpp)); \
    break; \
    case Tport##Vector: \
    VECTOR_PTR(QVector<Tcpp>)->append(VALUE(QVector<Tcpp>)); \
    break;

#define CASE_SIZE(Tcpp, Tport) \
    case Tport: \
    return VECTOR_PTR(Tcpp)->size(); \
    break; \
    case Tport##Vector: \
    return VECTOR_PTR(QVector<Tcpp>)->size(); \
    break;

#define CASE_TO_STRING(Tcpp, Tport, row, separator) \
    case Tport: \
    Q_ASSERT(row < VECTOR_PTR(Tcpp)->size()); \
    return convert<QString>( VECTOR_PTR(Tcpp)->at(row) ); \
    case Tport##Vector: \
    Q_ASSERT(row < VECTOR_PTR(QVector<Tcpp>)->size()); \
    sl = convert<QStringList>( VECTOR_PTR(QVector<Tcpp>)->at(row) ); \
    return sl.join(separator);

namespace boxes {

Vector::Vector(Port *parent)
    : _vectorType(Null), _vectorPtr(0), _parent(parent)
{
    Q_ASSERT(_parent);
}

Vector::~Vector() {
    erase();
}

void Vector::reserve(int size) {
    Q_ASSERT(!_vectorPtr);
    _vectorType = _parent->type();
    Q_ASSERT(_vectorType != Null);
    switch(_vectorType) {
    CASE_RESERVE(bool, Bool, size)
    CASE_RESERVE(char, Char, size)
    CASE_RESERVE(int, Int, size)
    CASE_RESERVE(long int, LongInt, size)
    CASE_RESERVE(long long int, LongLongInt, size)
    CASE_RESERVE(float, Float, size)
    CASE_RESERVE(double, Double, size)
    CASE_RESERVE(long double, LongDouble, size)
    CASE_RESERVE(QString, String, size)
    CASE_RESERVE(QDate, Date, size)
    CASE_RESERVE(QTime, Time, size)
    CASE_RESERVE(QDateTime, DateTime, size)
    case Null: throw Exception("Error: Vector::reserve found 'Null' type");
    }
}

void Vector::erase() {
    switch(_vectorType) {
    CASE_ERASE(bool, Bool)
    CASE_ERASE(char, Char)
    CASE_ERASE(int, Int)
    CASE_ERASE(long int, LongInt)
    CASE_ERASE(long long int, LongLongInt)
    CASE_ERASE(float, Float)
    CASE_ERASE(double, Double)
    CASE_ERASE(long double, LongDouble)
    CASE_ERASE(QString, String)
    CASE_ERASE(QDate, Date)
    CASE_ERASE(QTime, Time)
    CASE_ERASE(QDateTime, DateTime)
    case Null: if (_vectorPtr != 0) throw Exception("Error: Vector::erase found 'Null' type with _valuePtr!=0");
    }
    _vectorPtr = 0;
}

void Vector::append(const void *valuePtr) {
    Q_ASSERT(valuePtr);
    switch(_vectorType) {
    CASE_APPEND(bool, Bool)
    CASE_APPEND(char, Char)
    CASE_APPEND(int, Int)
    CASE_APPEND(long int, LongInt)
    CASE_APPEND(long long int, LongLongInt)
    CASE_APPEND(float, Float)
    CASE_APPEND(double, Double)
    CASE_APPEND(long double, LongDouble)
    CASE_APPEND(QString, String)
    CASE_APPEND(QDate, Date)
    CASE_APPEND(QTime, Time)
    CASE_APPEND(QDateTime, DateTime)
    case Null: throw Exception("Error: Vector::append found 'Null' type");
    }
}

int Vector::size() const {
    switch(_vectorType) {
    CASE_SIZE(bool, Bool)
    CASE_SIZE(char, Char)
    CASE_SIZE(int, Int)
    CASE_SIZE(long int, LongInt)
    CASE_SIZE(long long int, LongLongInt)
    CASE_SIZE(float, Float)
    CASE_SIZE(double, Double)
    CASE_SIZE(long double, LongDouble)
    CASE_SIZE(QString, String)
    CASE_SIZE(QDate, Date)
    CASE_SIZE(QTime, Time)
    CASE_SIZE(QDateTime, DateTime)
    case Null: break;
    }
    return 0;
}

QString Vector::toString(int row, QString separator) const {
    Q_ASSERT(row > -1);
    QStringList sl;
    switch(_vectorType) {
    CASE_TO_STRING(bool, Bool, row, separator)
    CASE_TO_STRING(char, Char, row, separator)
    CASE_TO_STRING(int, Int, row, separator)
    CASE_TO_STRING(long int, LongInt, row, separator)
    CASE_TO_STRING(long long int, LongLongInt, row, separator)
    CASE_TO_STRING(float, Float, row, separator)
    CASE_TO_STRING(double, Double, row, separator)
    CASE_TO_STRING(long double, LongDouble, row, separator)
    CASE_TO_STRING(QString, String, row, separator)
    CASE_TO_STRING(QDate, Date, row, separator)
    CASE_TO_STRING(QTime, Time, row, separator)
    CASE_TO_STRING(QDateTime, DateTime, row, separator)
    case Null: throw Exception("Error: Vector::toString found 'Null' type");
    }
    return QString();
}

PortType Vector::type() const {
    return _vectorType;
}

const void * Vector::ptr() const {
    return _vectorPtr;
}

}
