#include "port_access.h"

namespace base {

#define CASE_PORT_ACCESS(x) case PortAccess::x: return #x

template<> QString convert(base::PortAccess access) {
    switch (access) {
        CASE_PORT_ACCESS(Uninitialized);
        CASE_PORT_ACCESS(Input);
        CASE_PORT_ACCESS(Output);
    }
    return QString();
}

}

