#include <base/publish.h>
#include "area_loss_respiration.h"

using namespace base;

namespace saccharina {

PUBLISH(AreaLossRespiration)

AreaLossRespiration::AreaLossRespiration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the loss of area by respiration when C<Cmin");
    Input(A).imports("area[value]");
    Input(C).imports("C[value]");
    Input(Cmin).imports("fcarbon[Cmin]");
    Input(Cstruct).imports("biomass[Cstruct]");
    Output(value).unit("dm2").help("Area lost");
}

void AreaLossRespiration::reset() {
   update();
}

void AreaLossRespiration::update() {
    value = (C<Cmin) ? A*(Cmin-C)/Cstruct : 0.;
}

}
