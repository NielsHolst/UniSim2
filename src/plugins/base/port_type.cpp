#include <algorithm>
#include <QList>
#include <QMap>
#include <QSet>
#include <QVector>
#include "convert.h"
#include "exception.h"
#include "port_type.h"

namespace base {

#define NAMEOF_PORTTYPE(Y,Z) \
    case Z: \
        s = #Y; \
        break; \
    case Z##Vector: \
        s = "QVector<" #Y ">"; \
        break;

QString nameOf(PortType type) {
    QString s;
    switch (type) {
        NAMEOF_PORTTYPE(bool, Bool)
        NAMEOF_PORTTYPE(char, Char)
        NAMEOF_PORTTYPE(int, Int)
        NAMEOF_PORTTYPE(long int, LongInt)
        NAMEOF_PORTTYPE(long long int, LongLongInt)
        NAMEOF_PORTTYPE(float, Float)
        NAMEOF_PORTTYPE(double, Double)
        NAMEOF_PORTTYPE(long double, LongDouble)
        NAMEOF_PORTTYPE(QString, String)
        NAMEOF_PORTTYPE(QDate, Date)
        NAMEOF_PORTTYPE(QTime, Time)
        NAMEOF_PORTTYPE(QDateTime, DateTime)
        case Null:
            s = "Null";
    }
    return s;
}

bool isVector(PortType type) {
    return int(type)%2 == 1;
}

PortType asVector(PortType type) {
    if (type==Null)
        ThrowException("Cannot convert 'Null' to vector type");
    int i(type);
    return (i%2 == 0) ? PortType(i+1) : type;
}

PortType asScalar(PortType type) {
    if (type==Null)
        ThrowException("Cannot convert 'Null' to scalar type");
    int i(type);
    return (i%2 == 1) ? PortType(i-1) : type;
}

template<class T> inline bool isType(QString s) {
    try {
        convert<T>(s);
        return true;
    }
    catch(Exception &) {
    }
    return false;
}

namespace {
    bool isVector(QString s) {
        s = s.trimmed();
        return s.startsWith("(") && s.endsWith(")");
    }

    class Promotion {
        typedef QList<PortType> Rule;
        QMap<PortType, Rule> _rules;
    public:
        Promotion() {
            _rules[Bool] =        Rule() << String;
            _rules[Char] =        Rule() << String;
            _rules[Int]  =        Rule() << LongInt << LongLongInt << Double << LongDouble << String;
            _rules[LongInt] =     Rule() << LongLongInt << Double << LongDouble << String;
            _rules[LongLongInt] = Rule() << Double << LongDouble << String;
            _rules[Float] =       Rule() << Double << LongDouble << String;
            _rules[Double] =      Rule() << LongDouble << String;
            _rules[LongDouble] =  Rule() << String;
            _rules[String] =      Rule();
            _rules[Date] =        Rule() << String;
            _rules[DateTime] =    Rule() << String;
            _rules[Time] =        Rule() << String;
            _rules[Null] =        Rule() << String;
        }

        PortType common(PortType a, PortType b) {
            // Check
            if (isVector(a) || isVector(b))
                ThrowException("Vector of vectors is not allowed");
            // Types are equal
            if (a == b)
                return a;
            // One type is compatible with the other
            bool aIsCompatible = _rules.value(a).contains(b),
                 bIsCompatible = _rules.value(b).contains(a);
            if (aIsCompatible)
                return b;
            else if (bIsCompatible)
                return a;
            // Find lowest common type
            QSet<PortType> A = QSet<PortType>::fromList(_rules.value(a)),
                           B = QSet<PortType>::fromList(_rules.value(b)),
                           C = A.intersect(B);
            Q_ASSERT(!C.isEmpty());
            QList<PortType> candidates = QList<PortType>::fromSet(C);
            std::sort(candidates.begin(), candidates.end());
            return candidates.first();
        }
    };


    PortType common(QList<PortType> types) {
        static Promotion promotion;
        PortType type = types.at(0);
        for (PortType next : types) {
            type = promotion.common(next, type);
        }
        return type;
    }

    PortType deducePortTypeVector(QString s) {
        QStringList list = split(s);
        if (list.isEmpty())
            return StringVector;
        QList<PortType> types;
        for (QString s : list)
            types << deducePortType(s);
        return asVector(common(types));
    }
}

PortType deducePortType(QString s) {
    if (isVector(s)) return deducePortTypeVector(s);
    if (isType<bool>(s)) return Bool;
    if (isType<char>(s)) return Char;
    if (isType<int>(s)) return Int;
    if (isType<long int>(s)) return LongInt;
    if (isType<long long int>(s)) return LongLongInt;
    if (isType<double>(s)) return Double;
    if (isType<long double>(s)) return LongDouble;
    if (isType<QDate>(s)) return Date;
    if (isType<QDateTime>(s)) return DateTime;
    if (isType<QTime>(s)) return Time;
    return String;
}

}
