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

ValueCollection::ValueCollection() : _buffer(nullptr),_isEmpty(true) {
}

ValueCollection::ValueCollection(QVector<const Value*> _values) : ValueCollection() {
    initialize(_values);
};

void ValueCollection::createBuffer(Value::Type type) {
    using Type = Value::Type;
    switch (type) {
    case Type::Bool         : _buffer = new vbool     ; break;
    case Type::Int          : _buffer = new vint      ; break;
    case Type::Double       : _buffer = new vdouble   ; break;
    case Type::String       : _buffer = new vQString  ; break;
    case Type::Date         : _buffer = new vQDate    ; break;
    case Type::Time         : _buffer = new vQTime    ; break;
    case Type::DateTime     : _buffer = new vQDateTime; break;
    case Type::BareDate     : _buffer = new vBareDate ; break;
    default: ThrowException("Illegal buffer type").value(Value::typeName(type));
    }
}

ValueCollection::~ValueCollection() {
    using Type = Value::Type;
    switch (_commonType) {
    case Type::Bool         : delete static_cast<vbool     *>(_buffer); break;
    case Type::Int          : delete static_cast<vint      *>(_buffer); break;
    case Type::Double       : delete static_cast<vdouble   *>(_buffer); break;
    case Type::String       : delete static_cast<vQString  *>(_buffer); break;
    case Type::Date         : delete static_cast<vQDate    *>(_buffer); break;
    case Type::Time         : delete static_cast<vQTime    *>(_buffer); break;
    case Type::DateTime     : delete static_cast<vQDateTime*>(_buffer); break;
    case Type::BareDate     : delete static_cast<vBareDate *>(_buffer); break;
    default: ;
    }
}

void ValueCollection::initialize() {
    if (_original.isEmpty()) {
        _isEmpty = true;
        return;
    }
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
        case Type::Bool         : collect<bool     >(); _commonType = baseType; break;
        case Type::Int          : collect<int      >(); _commonType = baseType; break;
        case Type::Double       : collect<double   >(); _commonType = baseType; break;
        case Type::String       : collect<QString  >(); _commonType = baseType; break;
        case Type::Date         : collect<QDate    >(); _commonType = baseType; break;
        case Type::Time         : collect<QTime    >(); _commonType = baseType; break;
        case Type::DateTime     : collect<QDateTime>(); _commonType = baseType; break;
        case Type::BareDate     : collect<BareDate >(); _commonType = baseType; break;
        default: ThrowException("Illegal type").value(Value::typeName(baseType));
        }
    }
    else if (baseTypes.size() > 1) {
        _isEmpty = false;
        _hasOneType = false;
        if (baseTypes == _toInt.at(0))
            _commonType = Type::Int;
        else if (baseTypes == _toDouble.at(0) || baseTypes == _toDouble.at(1) || baseTypes == _toDouble.at(2))
            _commonType = Type::Double;
        else
            _commonType = Type::String;
        _valuePtrs = _original;
    }
    createBuffer(_commonType);
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
}

COLLECT(bool)
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
}

void ValueCollection::update() {
    if (_hasOneType)
        updateOneType();
    else
        updateMixedTypes();
}

#define UPDATE_ONE(T, V) \
    buffer<T>()->clear(); \
    for (auto v : std::get<QVector<const T*>>(_valuePtrs)) \
        *buffer<T>() << *v; \
    break

void ValueCollection::updateOneType() {
    using Type = Value::Type;
    switch (_commonType) {
    case Type::Uninitialized : ThrowException("ValueCollection is uninitialized");
    case Type::Bool          : UPDATE_ONE(bool     , vbool     );
    case Type::Int           : UPDATE_ONE(int      , vint      );
    case Type::Double        : UPDATE_ONE(double   , vdouble   );
    case Type::String        : UPDATE_ONE(QString  , vQString  );
    case Type::Date          : UPDATE_ONE(QDate    , vQDate    );
    case Type::Time          : UPDATE_ONE(QTime    , vQTime    );
    case Type::DateTime      : UPDATE_ONE(QDateTime, vQDateTime);
    case Type::BareDate      : UPDATE_ONE(BareDate , vBareDate );
    default: ThrowException("Illegal type").value(Value::typeName(_commonType));
    }
}

void ValueCollection::updateMixedTypes() {
//    using Type = Value::Type;
//    switch (_commonType) {
//    case Type::Int    : updateMixed<int    , vint    >(); break;
//    case Type::Double : updateMixed<double , vdouble >(); break;
//    case Type::String : updateMixed<QString, vQString>(); break;
//    default: ThrowException("Illegal common type").value(Value::typeName(_commonType));
//    }
}

bool ValueCollection::isEmpty() const {
    return _isEmpty;
}

const Value& ValueCollection::values() {
    using Type = Value::Type;
    switch (_commonType) {
    case Type::Uninitialized : ThrowException("Buffer is uninitialized");
    case Type::Bool          : _values = buffer<bool     >(); break;
    case Type::Int           : _values = buffer<int      >(); break;
    case Type::Double        : _values = buffer<double   >(); break;
    case Type::String        : _values = buffer<QString  >(); break;
    case Type::Date          : _values = buffer<QDate    >(); break;
    case Type::Time          : _values = buffer<QTime    >(); break;
    case Type::DateTime      : _values = buffer<QDateTime>(); break;
    case Type::BareDate      : _values = buffer<BareDate >(); break;
    default: ThrowException("Illegal type for value").value(Value::typeName(_commonType));
    }
    return _values;
}

Value::Type ValueCollection::type() const {
    return static_cast<Value::Type>( static_cast<int>(_commonType) +
                                     static_cast<int>(Value::Type::VecBool) - 1 );
}

}
