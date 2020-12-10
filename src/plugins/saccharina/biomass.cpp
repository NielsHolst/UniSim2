#include <base/publish.h>
#include "biomass.h"

using namespace base;

namespace saccharina {

PUBLISH(Biomass)

/*
 * Biomass skal omskrives fuldstændigt
 */

Biomass::Biomass(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the yield, weight, C and N percent of the system");
    Input(structuralMass).imports("structure[mass]");
    Input(structuralMassGrowth).imports("allocation[structuralMassGrowth]");
    Input(kA).equals(0.6).unit("g structure/dm2").help("Structural dry weight per unit area");
    Input(Cstruct).equals(0.20).unit("g C/g structure").help("Amount of structurally bound carbon");
    Input(Nstruct).equals(0.01).unit("g N/g structure").help("Amount of structurally bound nitrogen");
    Input(kdw).equals(0.0785).help("Dry weight to wet weight ratio");
    Input(kN).equals(2.72).help("Mass of nitrogen reserves per gram nitrogen");
    Input(kC).equals(2.1213).help("Mass of carbon reserves per gram nitrogen");
    Input(C).imports("reserves/carbon[proportion]");
    Input(N).imports("reserves/nitrogen[proportion]");
    Input(plantDensity).equals(1.).unit("per m rope").help("Plant density");
    Output(dryWeight).unit("g DW").help("Dry weight per plant");
    Output(dryWeightGrowth).unit("g DW/h").help("Dry weight growth increment");
    Output(wetWeight).unit("g WW").help("Wet weight per plant");
    Output(carbonWeight).unit("g C").help("Carbon total weight");
    Output(nitrogenWeight).unit("g N").help("Nitrogen total weight");
    Output(carbonPct).unit("%").help("Percentage carbon content (by dry weight)");
    Output(nitrogenPct).unit("%").help("Percentage nitrogen content (by dry weight)");
    Output(CNratio).help("C/N ratio");
    Output(dryWeightYield).unit("g DW per m rope").help("Standing yield (dry weight)");
    Output(wetWeightYield).unit("g WW per m rope").help("Standing yield (wet weight)");
}

void Biomass::reset() {
   update();
}

void Biomass::update() {

   dryWeight = structuralMass * (1 + C*kC + N*kN);
   dryWeightGrowth = structuralMassGrowth * (1 + C*kC + N*kN);
   wetWeight = structuralMass * ((1/kdw) + C*kC + N*kN);
   carbonWeight = structuralMass * (C + Cstruct);
   nitrogenWeight = structuralMass * (N + Nstruct);
   carbonPct = (dryWeight>0.) ? 100.*(carbonWeight/dryWeight) : 0.;
   nitrogenPct = (dryWeight>0.) ? 100.*(nitrogenWeight/dryWeight) : 0.;
   CNratio = (nitrogenWeight>0.) ? carbonWeight/nitrogenWeight : 0.;
   dryWeightYield = dryWeight*plantDensity;
   wetWeightYield = wetWeight*plantDensity;
}


}

