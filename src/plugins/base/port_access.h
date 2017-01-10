#ifndef BASE_PORT_ACCESS_H
#define BASE_PORT_ACCESS_H

#include "exception.h"

namespace base {

enum class PortAccess{Uninitialized, Input, Output};

template<class T> T convert(PortAccess) {
    ThrowException("Cannot convert PortAccess");
    return T();
}

template<> QString convert(PortAccess access);

}
#endif

