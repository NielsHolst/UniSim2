#include <base/publish.h>
#include "plot_r.h"

using namespace base;

namespace boxes {

PUBLISH(PlotR)

PlotR::PlotR(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(PlotR);
}


}
