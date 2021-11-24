/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VALUE_TYPED_H
#define BASE_VALUE_TYPED_H

#include "convert.h"
#include "exception.h"

namespace base {

template <class T>
class ValueTyped {
public:
    ValueTyped()            : _foreign(nullptr), _own(nullptr), _isInitialized(false) { }

    ValueTyped(T *variable) { initialize(variable); }

    ValueTyped(T value)     { initialize(value); }

    ~ValueTyped()           { delete _own; }

    // allocate variable of type T

    void initialize(T *variable)
    // Link to foreign variable
    {
        if (_isInitialized)
            ThrowException("Value already initialized");
        _foreign = variable;
        _isInitialized = true;
    }

    void initialize(T value)
    // Link to own variable
    {
        if (_isInitialized)
            ThrowException("Value already initialized");
        _own = new T;
        *_own = value;
        _isInitialized = true;
    }

    template <class U> void changeValue(U value)
    // Set to value with compatible type U
    {
        Q_ASSERT(_isInitialized);
        if (_foreign)
            *_foreign = convert<T>(value);
        else
            *_own = convert<T>(value);
    }

    const T* valuePtr() const
    // Return pointer in native type T
    {
        Q_ASSERT(_isInitialized);
        return _foreign ? _foreign : _own;
    }

    template <class U> U value() const
    // Get value in compatible type U
    {
        Q_ASSERT(_isInitialized);
        return convert<U>(*valuePtr());
    }

private:
    T *_foreign, *_own;
    bool _isInitialized;
};

}
#endif
