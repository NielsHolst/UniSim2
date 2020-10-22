#include <base/publish.h>
#include "demand_carbon_exudation.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandCarbonExudation)

DemandCarbonExudation::DemandCarbonExudation(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates carbon exudation");
    Input(gamma).help("Exudation parameter (h-1)");
    Input(reservesProportion).imports("reserves/carbon[proportion]");
    Input(structuralMass).imports("structure[mass]");
    Input(demandStructure).imports("../structure[value]");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(value).unit("g C").help("Carbon exudation");
    Output(proportion).unit("[0;1]").help("The proportion exuded");
}

void DemandCarbonExudation::reset() {
   update();
}

void DemandCarbonExudation::update() {
    double dt = timeStepSecs/3600.;
    proportion = 1. - exp(-gamma*dt);
    value = proportion*(reservesProportion*structuralMass + demandStructure);
}

}
