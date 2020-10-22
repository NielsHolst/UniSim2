#include <base/phys_math.h>
#include <base/publish.h>
#include "allocation.h"

using namespace base;
using phys_math::minmax;

namespace saccharina {

PUBLISH(Allocation)

Allocation::Allocation(QString name, QObject *parent)
    : Box(name, parent)
{
    help("allocates C and N supplies");
    Input(structuralProportionC).imports("/sim/structure[proportionC]");
    Input(structuralProportionN).imports("/sim/structure[proportionN]");
    Input(structuralMass).imports("structure[mass]");
    Input(curReservesProportionC).imports("reserves/carbon[proportion]");
    Input(curReservesProportionN).imports("reserves/nitrogen[proportion]");
    Input(reservesMaxProportionC).imports("reserves/carbon[maxProportion]");
    Input(reservesMaxProportionN).imports("reserves/nitrogen[maxProportion]");
    Input(supplyCarbonTotal).imports("photosynthesis[supply]");
    Input(supplyNitrogenTotal).imports("nitrogenUptake[supply]");
    Input(demandCarbonRespiration).imports("demand/carbon/respiration[value]");
    Input(demandCarbonStructure).imports("demand/carbon/structure[value]");
    Input(demandNitrogenStructure).imports("demand/nitrogen/structure[value]");
    Input(proportionExuded).imports("demand/carbon/exudation[proportion]");

    Output(MC0).unit("g C").help("Metabolic carbon pool step 0");
    Output(MC1).unit("g C").help("Metabolic carbon pool step 1");
    Output(MC2).unit("g C").help("Metabolic carbon pool step 2");
    Output(MN0).unit("g N").help("Metabolic nitrogen pool step 0");
    Output(supplyCarbonStructure).unit("g C").help("Carbon supply to structural growth");
    Output(supplyCarbonExudation).unit("g C").help("Carbon supply to exudation");
    Output(structuralMassGrowth).unit("g structure").help("Growth in structural mass");
    Output(structuralMassLoss).unit("g structire").help("Loss of structural mass to excess respiration");
    Output(structuralMassLossRate).unit("[0;1]").help("Proportion of structural mass to excess respiration");
    Output(reservesProportionC).unit("[0;1]").help("New proportion of carbon reserves");
    Output(reservesProportionN).unit("[0;1]").help("New proportion of nitrogen reserves");
    Output(sdCarbon).unit("[0;1]").help("Supply/demand ratio of carbon for structural growth");
    Output(sdNitrogen).unit("[0;1]").help("Supply/demand ratio of nitrogen for structural growth");
}

void Allocation::reset() {
    update();
}

inline double min(double a, double b, double c) {
    return std::min(std::min(a,b), c);
}

void Allocation::update() {
    MC0 = curReservesProportionC*structuralMass + supplyCarbonTotal;
    MN0 = curReservesProportionN*structuralMass + supplyNitrogenTotal;
    MC1 = MC0 - demandCarbonRespiration;
    sdNitrogen = (demandNitrogenStructure==0.) ? 0. : MN0/demandNitrogenStructure;
    if (MC1 < 0.) {
        structuralMassLoss = -MC1/structuralProportionC;
        structuralMassLossRate = structuralMassLoss/structuralMass;
        sdCarbon = 0;
        supplyCarbonStructure = 0.;
        MC2 = 0;
        MC3 = 0;
        supplyCarbonExudation = 0.;
        structuralMassGrowth = 0.;
        reservesProportionC = 0.;
        reservesProportionN = curReservesProportionN;
    }
    else {
        structuralMassLoss = 0.;
        structuralMassLossRate = 0.;
        sdCarbon   = (demandCarbonStructure==0.) ? 0. : MC1/(1. + proportionExuded)/demandCarbonStructure;
        supplyCarbonStructure = min(sdCarbon, sdNitrogen, 1.)*demandCarbonStructure;
        MC2 = MC1 - (1. + proportionExuded)*supplyCarbonStructure;
        double structExudation = std::min(proportionExuded*structuralProportionC*structuralMass, MC2);
        MC3 = MC2 - structExudation;
        supplyCarbonExudation = proportionExuded*supplyCarbonStructure + structExudation;
        structuralMassGrowth = supplyCarbonStructure/structuralProportionC;
        // Update reserves
        double newStructuralMass = structuralMass + structuralMassGrowth;
        reservesProportionC = minmax(0., MC3/newStructuralMass, reservesMaxProportionC);
        reservesProportionN = minmax(0., MN0/newStructuralMass - structuralProportionN, reservesMaxProportionN);
    }
}

}
