#ifndef BASE_PORT_MODE_H
#define BASE_PORT_MODE_H

#include "exception.h"

namespace base {

enum class PortMode{Default, Fixed, Referenced, MaybeReferenced};

template<class T> T convert(PortMode) {
    ThrowException("Cannot convert PortMode");
    return T();
}

template<> QString convert(PortMode mode);

}
#endif

