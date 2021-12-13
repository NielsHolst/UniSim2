/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VALUE_COLLECTION_H
#define BASE_VALUE_COLLECTION_H

#include <variant>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include "bare_date.h"
#include "exception.h"
#include "value.h"
//#include "value_typed.h"

namespace base {

class ValueCollection {
public:
    ValueCollection();
    ValueCollection(QVector<const Value*> _values);

    void initialize();
    void initialize(QVector<const Value*> _values);
    // Translate _original into _variant

    template <class T> void collect();
    // Collect values as a vector pointers to scalar type T;
    // allocate values buffer for type T

    void collect(Value::Type commonType);
    // Collect values as a vector pointers to generic values;
    // allocate values buffer for common type

    void update();
    // Update buffered values

    bool isEmpty() const;
    // Is collection empty?

    const Value& values();
    // Get values collected into one value


private:
    // Pointers to original values
    QVector<const Value*> _original;

    // Pointers translated to common types, if possible
    std::variant<
        std::monostate,
        // If _original elements were all of the same base type
        QVector<const bool*     >,
        QVector<const int*      >,
        QVector<const double*   >,
        QVector<const QString*  >,
        QVector<const QDate*    >,
        QVector<const QTime*    >,
        QVector<const QDateTime*>,
        QVector<const BareDate* >,
        // If _original elements were mixed base type
        QVector<const Value*>
    >
    _valuePtrs;

    // Vector buffer updated from _valuePtrs
    std::variant<
        std::monostate,
        // If _original elements are all of the same base type
        QVector<bool     >,
        QVector<int      >,
        QVector<double   >,
        QVector<QString  >,
        QVector<QDate    >,
        QVector<QTime    >,
        QVector<QDateTime>,
        QVector<BareDate >
    > _buffer;

    // If _original were of same base type then _hasOneType is true,
    // and _commonType contains the common base type
    bool _hasOneType, _isEmpty;
    Value::Type _commonType;
    Value _values;

    // Promotion rules; in case of mixed base types
    static QVector<QSet<Value::Type>> _toInt, _toDouble, _toString;

    void updateOneType();
    void updateMixedTypes();
    template<class T, class VT> void updateMixed();
};

template <> void ValueCollection::collect<bool     >();
template <> void ValueCollection::collect<int      >();
template <> void ValueCollection::collect<double   >();
template <> void ValueCollection::collect<QString  >();
template <> void ValueCollection::collect<QDate    >();
template <> void ValueCollection::collect<QTime    >();
template <> void ValueCollection::collect<QDateTime>();
template <> void ValueCollection::collect<BareDate >();

template<class T, class VT> void ValueCollection::updateMixed() {
//    using Values = QVector<const Value*>;
//    Values ptrs = std::get<Values>(_valuePtrs);
//    QVector<T> values;
//    for (const Value *pvalue : ptrs) {
//        if (pvalue->isVector())
//            values << convert<VT>(*pvalue);
//        else
//            values << convert<T>(*pvalue);
//    }
//    _values = values;
}

}
#endif
