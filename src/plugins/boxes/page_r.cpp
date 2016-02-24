#include <base/publish.h>
#include "page_r.h"

using namespace base;

namespace boxes {

PUBLISH(PageR)

PageR::PageR(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(PageR);
    Input(xAxis).imports("/*[step]");
    Input(width).equals(14);
    Input(height).equals(10);
    Input(ncol).equals(1);
}

}
