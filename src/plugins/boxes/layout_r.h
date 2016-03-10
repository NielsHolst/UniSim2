#ifndef LAYOUT_R_H
#define LAYOUT_R_H

#include <QMap>
#include <base/convert.h>
#include <base/exception.h>

namespace boxes {
    enum LayoutR {Merged, Facetted};
}

namespace base  {
    template<> boxes::LayoutR convert(QString source);
    template<class T> T convert(boxes::LayoutR source); // undefined
    template<> QString convert(boxes::LayoutR source);
}

#endif
