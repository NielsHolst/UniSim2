/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QSet>
#include <QStringList>
#include "convert.h"
#include "value_collection.h"

namespace base {

// Define subsets of compatible types and their destination type
using ValueSet  = QSet<Value::Type>;
using ValueSets = QVector<ValueSet>;
ValueSets
    ValueCollection::_toInt = ValueSets {
        ValueSet() << Value::Type::Bool << Value::Type::Int
    },
    ValueCollection::_toDouble = ValueSets {
        ValueSet() << Value::Type::Bool << Value::Type::Double,
        ValueSet() << Value::Type::Int << Value::Type::Double,
        ValueSet() << Value::Type::Bool << Value::Type::Int << Value::Type::Double
    };

ValueCollection::ValueCollection() : _isEmpty(true) {
}

ValueCollection::ValueCollection(QVector<const Value*> _values) : ValueCollection() {
    initialize(_values);
};

void ValueCollection::initialize() {
    using Type = Value::Type;
    QSet<Type> baseTypes;
    for (const Value *value : _original) {
        baseTypes << value->baseType();
    }
    if (baseTypes.size() == 1) {
        _isEmpty = false;
        _hasOneType = true;
        Type baseType = *baseTypes.cbegin();
        switch (baseType) {
        case Type::Bool         : collect<bool     >(); _commonType = baseType; return;
        case Type::Int          : collect<int      >(); _commonType = baseType; return;
        case Type::Double       : collect<double   >(); _commonType = baseType; return;
        case Type::String       : collect<QString  >(); _commonType = baseType; return;
        case Type::Date         : collect<QDate    >(); _commonType = baseType; return;
        case Type::Time         : collect<QTime    >(); _commonType = baseType; return;
        case Type::DateTime     : collect<QDateTime>(); _commonType = baseType; return;
        case Type::BareDate     : collect<BareDate >(); _commonType = baseType; return;
        default: ;
        }
        ThrowException("Illegal type").value(Value::typeName(baseType));
    }
    else if (baseTypes.size() > 1) {
        _isEmpty = false;
        _hasOneType = false;
        if (baseTypes == _toInt.at(0))
            _commonType = Type::Int;
        else if (baseTypes == _toDouble.at(0) || baseTypes == _toDouble.at(1) || baseTypes == _toDouble.at(2))
            _commonType = Type::Double;
        else if (baseTypes.contains(Type::String))
            _commonType = Type::String;
        collect(_commonType);
    }
}

void ValueCollection::initialize(QVector<const Value*> _values) {
    _original.clear();
    _original = _values;
    initialize();
}

#define COLLECT(T) \
template <> void ValueCollection::collect<T>() { \
    using Ptrs = QVector<const T*>; \
    Ptrs ptrs; \
    for (const Value *value : _original) { \
        if (value->isVector()) { \
            auto vec = value->valuePtr<QVector<T>>(); \
            const T *p = vec->constData(); \
            int n = vec->size(); \
            for (int i=0; i<n; ++i) \
                ptrs << p++; \
        } \
        else { \
            ptrs << value->valuePtr<T>(); \
        } \
    } \
    _valuePtrs = ptrs; \
    _buffer = QVector<T>(ptrs.size()); \
}

COLLECT(bool     )
//COLLECT(int      )
COLLECT(double   )
COLLECT(QString  )
COLLECT(QDate    )
COLLECT(QTime    )
COLLECT(QDateTime)
COLLECT(BareDate )

template <> void ValueCollection::collect<int>() {
    using Ptrs = QVector<const int*>;
    Ptrs ptrs;
    for (const Value *value : _original) {
        if (value->isVector()) {
            auto vec = value->valuePtr<QVector<int>>();
            const int *p = vec->constData();
            int n = vec->size();
            for (int i=0; i<n; ++i)
                ptrs << p++;
        }
        else {
            ptrs << value->valuePtr<int>();
        }
    }
    _valuePtrs = ptrs;
    _buffer = QVector<int>(ptrs.size());

    const QVector<int> *x = std::get_if<QVector<int>>(&_buffer);
    QVector<int> *y;
    _values.initialize(y);
    _values.initialize(x);
}

void ValueCollection::collect(Value::Type commonType) {
    _valuePtrs = _original;
    int n = 0;
    for (const Value *value : _original) {
        n += value->size();
    }
    using Type = Value::Type;
    switch (commonType) {
    case Type::Int    :
        _buffer = QVector<int    >(n);
        _values.initialize(std::get_if<QVector<int>>(&_buffer));
        break;
    case Type::Double :
        _buffer = QVector<double >(n);
        _values.initialize(std::get_if<QVector<double>>(&_buffer));
        break;
    case Type::String :
        _buffer = QVector<QString>(n);
        _values.initialize(std::get_if<QVector<QString>>(&_buffer));
        break;
    default:
        ThrowException("Illegal common type").value(Value::typeName(commonType));
    }
}

void ValueCollection::update() {
    if (_hasOneType)
        updateOneType();
    else
        updateMixedTypes();
}

void ValueCollection::updateOneType() {
    auto dest = std::get<QVector<int>>(_buffer).data();
    for (auto v : std::get<QVector<const int*>>(_valuePtrs))
        *dest++ = *v;

//    using Type = Value::Type;
//    switch (_commonType) {
//    case Type::Uninitialized : ThrowException("ValueCollection is uninitialized");
//    case Type::Bool     : _values = convert<vbool     >(_valuePtrs); break;
//    case Type::Int      : break; //_values = std::get<QVector<const int*>>(_valuePtrs); break;
//    case Type::Double   : _values = convert<vdouble   >(_valuePtrs); break;
//    case Type::String   : _values = convert<vQString  >(_valuePtrs); break;
//    case Type::Date     : _values = convert<vQDate    >(_valuePtrs); break;
//    case Type::Time     : _values = convert<vQTime    >(_valuePtrs); break;
//    case Type::DateTime : _values = convert<vQDateTime>(_valuePtrs); break;
//    case Type::BareDate : _values = convert<vBareDate >(_valuePtrs); break;
//    default: ThrowException("Illegal type").value(Value::typeName(_commonType));
//    }
}

void ValueCollection::updateMixedTypes() {
    using Type = Value::Type;
    switch (_commonType) {
    case Type::Int    : updateMixed<int    , vint    >(); break;
    case Type::Double : updateMixed<double , vdouble >(); break;
    case Type::String : updateMixed<QString, vQString>(); break;
    default: ThrowException("Illegal common type").value(Value::typeName(_commonType));
    }
}

bool ValueCollection::isEmpty() const {
    return _isEmpty;
}

const Value& ValueCollection::values()  const {
    return _values;
}

}
