#include "port_mode.h"

namespace base {

#define CASE_PORT_MODE(x) case PortMode::x: return #x

template<> QString convert(base::PortMode mode) {
    switch (mode) {
        CASE_PORT_MODE(Default);
        CASE_PORT_MODE(Fixed);
        CASE_PORT_MODE(Referenced);
        CASE_PORT_MODE(MaybeReferenced);
    }
    return QString();
}

}

