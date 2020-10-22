#include <base/publish.h>
#include "demand_nitrogen_structure.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandNitrogenStructure)

DemandNitrogenStructure::DemandNitrogenStructure(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates demand for structural nitrogen");
    Input(demandCarbonStructure).imports("demand/carbon/structure[value]");
    Input(proportionC).imports("/sim/structure[proportionC]");
    Input(proportionN).imports("/sim/structure[proportionN]");
    Output(value).unit("g N").help("Nitrogen demand for structural growth");
}

void DemandNitrogenStructure::reset() {
   update();
}

void DemandNitrogenStructure::update() {
    value = proportionN*demandCarbonStructure/proportionC;
}

}
