#include <base/publish.h>
#include "demand_carbon_structure.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandCarbonStructure)

DemandCarbonStructure::DemandCarbonStructure(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates demand for structural growth");
    Input(maxGrowthRate).equals(0.0075).unit("h-1").help("Maximum relative growth rate");
    Input(structuralMass).imports("structure[mass]");
    Input(fTemp).equals(1.).help("Temperature scaling of growth rate");
    Input(fSalinity).equals(1.).help("Salinity scaling of growth rate");
    Input(fArea).equals(1.).help("Area scaling of growth rate");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(value).unit("g C").help("Carbon demand for structural growth");
}

void DemandCarbonStructure::reset() {
   update();
}

void DemandCarbonStructure::update() {
    value = maxGrowthRate*structuralMass*fTemp*fSalinity*fArea*timeStep/3600.;
}

}