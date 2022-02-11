#include <base/publish.h>
#include "a.h"

using namespace base;

namespace test_boxes {

PUBLISH(A)

A::A(QString name, Box *parent)
    : Box(name, parent)
{
    Input(pbool).equals(false);
    Input(pint).imports("..[pint]");
    Input(pdouble).imports("../*[pint]");
    Input(pstring).imports(".../*[pstring]");
    Input(ppath);

}

}
