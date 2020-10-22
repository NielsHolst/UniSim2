#include <base/publish.h>
#include "area_loss_erosion.h"

using namespace base;

namespace saccharina {

PUBLISH(AreaLossErosion)

AreaLossErosion::AreaLossErosion(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the rate of area loss by erosion (eq. 6)");
    Input(A).imports("area[value]");
    Input(eps).equals(0.22).help("Frond erosion parameter");
    Output(value).unit("dm2/dm2/d").help("Relative loss rate");
}

void AreaLossErosion::reset() {
   update();
}

void AreaLossErosion::update() {
    double epsA = exp(eps*A);
    value = 1e-6*epsA/(1. + 1e-6*(epsA - 1.));
}

}
