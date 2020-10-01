#include <base/dialog.h>
#include <base/publish.h>
#include "growth_rate.h"

using namespace base;
using std::min;

namespace saccharina {

PUBLISH(GrowthRate)

GrowthRate::GrowthRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the specific growth rate of frond area (eq. 2)");
    Input(maxValue).equals(0.18).unit("dm2/dm2/d").help("Max. specific growth rate");
    Input(farea).imports("farea[value]");
    Input(ftemp).imports("ftemperature[value]");
    Input(fsalinity).imports("fsalinity[value]");
    Input(fnitrogen).imports("fnitrogen[value]");
    Input(fcarbon).imports("fcarbon[value]");
    Output(value).unit("dm2/dm2/d").help("Specific growth rate");
}

void GrowthRate::reset() {
   update();
}

void GrowthRate::update() {
    value = maxValue*farea*ftemp*fsalinity*min(fnitrogen, fcarbon);
}

}
