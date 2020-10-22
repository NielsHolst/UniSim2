#include <base/publish.h>
#include "demand_carbon_reserves.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandCarbonReserves)

DemandCarbonReserves::DemandCarbonReserves(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates demand for carbon reserves (new)");
    Input(maxReservesProportionC).imports("reserves/carbon[maxProportion]");
    Input(reservesProportionC).imports("reserves/carbon[proportion]");
    Input(structuralMass).imports("structure[mass]");
    Input(structProportionC).imports("sim/structure[proportionC]");
    Input(demandStructureC).imports("../structure[value]");
    Output(value).unit("g C").help("Demand for carbon reserves ");
}

void DemandCarbonReserves::reset() {
   update();
}

void DemandCarbonReserves::update() {
    value = (maxReservesProportionC - reservesProportionC)*structuralMass +
             maxReservesProportionC*demandStructureC/structProportionC;
}

}
