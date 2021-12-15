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
    ~ValueCollection();

    void initialize();
    void initialize(QVector<const Value*> _values);
    // Translate _original into _variant

    template <class T> void collect();
    // Collect values as a vector pointers to scalar type T;
    // allocate values buffer for type T

    void update();
    // Update buffered values

    bool isEmpty() const;
    // Is collection empty?

    const Value& values();
    // Get values collected into one value

    Value::Type type() const;
    // Type of values (always a vector type)


private:
    // Pointers to original values
    QVector<const Value*> _original;

    // Characterise original pointers
    bool _hasOneType, _isEmpty;

    // Pointers translated to common type
    std::variant<
        std::monostate,
        QVector<const bool*     >,
        QVector<const int*      >,
        QVector<const double*   >,
        QVector<const QString*  >,
        QVector<const QDate*    >,
        QVector<const QTime*    >,
        QVector<const QDateTime*>,
        QVector<const BareDate* >
    >
    _valuePtrs;

    // Vector buffer updated from _valuePtrs
    void *_buffer;
    void createBuffer(Value::Type type);
    template <class T> QVector<T>* buffer();

    // Holds return values
    Value _values;

    // _commonType contains the element type of the buffer
    Value::Type _commonType;

    // Promotion rules; in case of mixed base types
    static QVector<QSet<Value::Type>> _toInt, _toDouble, _toString;

    void updateOneType();
    void updateMixedTypes();
};

template <> void ValueCollection::collect<bool     >();
template <> void ValueCollection::collect<int      >();
template <> void ValueCollection::collect<double   >();
template <> void ValueCollection::collect<QString  >();
template <> void ValueCollection::collect<QDate    >();
template <> void ValueCollection::collect<QTime    >();
template <> void ValueCollection::collect<QDateTime>();
template <> void ValueCollection::collect<BareDate >();

template <> vbool     * ValueCollection::buffer<bool      >() { return static_cast<vbool     *>(_buffer); }
template <> vint      * ValueCollection::buffer<int       >() { return static_cast<vint      *>(_buffer); }
template <> vdouble   * ValueCollection::buffer<double    >() { return static_cast<vdouble   *>(_buffer); }
template <> vQString  * ValueCollection::buffer<QString   >() { return static_cast<vQString  *>(_buffer); }
template <> vQDate    * ValueCollection::buffer<QDate     >() { return static_cast<vQDate    *>(_buffer); }
template <> vQTime    * ValueCollection::buffer<QTime     >() { return static_cast<vQTime    *>(_buffer); }
template <> vQDateTime* ValueCollection::buffer<QDateTime >() { return static_cast<vQDateTime*>(_buffer); }
template <> vBareDate * ValueCollection::buffer<BareDate  >() { return static_cast<vBareDate *>(_buffer); }

}
#endif
