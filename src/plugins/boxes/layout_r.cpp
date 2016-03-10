#include "layout_r.h"

using namespace boxes;

namespace base {

template<> LayoutR convert(QString source) {
    if (source == "facetted")
        return Facetted;
    if (source == "merged")
        return Merged;
    throw Exception("Unknown layout. One of these expected (facetted, merged)", source);
}

template<> QString convert(LayoutR source) {
    switch (source) {
        case Facetted: return "facetted";
        case Merged: return "merged";
    }
    return QString();
}


}
