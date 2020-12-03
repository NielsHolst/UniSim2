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
    Input(Nmin).equals(0.01).help("minimum N reserve");
    Input(Cmin).equals(0.01).help("minimum C reserve");
    Input(C).imports("reserves/carbon[proportion]");
    Input(N).imports("reserves/nitrogen[proportion]");
    Input(plantDensity).equals(1.).unit("per m2").help("Plant density");
    Input(ropeDensity).equals(1.).unit("per m2").help("rope density");
    Output(Dryweight).unit("g DW").help("Dryweight");
    Output(DryweightprHour).unit("g DW").help("Dryweight");
    Output(Wetweight).unit("g DW").help("Wetweight");
    Output(carbonTotal).unit("g C").help("Carbon total weight");
    Output(nitrogenTotal).unit("g N").help("Nitrogen total weight");
    Output(nitrogenPct).unit("%").help("Percentage nitrogen content (by dry weight)");
    Output(carbonPct).unit("%").help("Percentage carbon content (by dry weight)");
    Output(DryweightprAlgae).unit("g DW pr algae").help("g DW per individual algae");
    Output(DryweightYieldprmeter).unit("g DW pr m cultivation rope").help("g DW pr m cultivation rope");
    Output(WetweightYieldprmeter).unit("g WW pr m cultivation rope").help("g WW pr m cultivation rope");
    Output(CNratio).help("C/N ratio");
}

void Biomass::reset() {
   update();
}

void Biomass::update() {

   Dryweight = structuralMass * (1 + (C-Cmin)*kC + Cmin + (N-Nmin)*kN + Nmin);
   DryweightprHour = structuralMassGrowth * (1 + (C-Cmin)*kC + Cmin + (N-Nmin)*kN + Nmin);
   Wetweight = structuralMass * ((1/kdw) + (C-Cmin)*kC + Cmin + (N-Nmin)*kN + Nmin);
   carbonTotal = structuralMass * (C + Cstruct);
   nitrogenTotal = structuralMass * (N + Nstruct);
   CNratio = (nitrogenTotal>0.) ? carbonTotal/nitrogenTotal : 0.;
   nitrogenPct = (Dryweight>0.) ? 100.*nitrogenTotal/Dryweight : 0.;
   carbonPct = (Dryweight>0.) ? 100.*carbonTotal/Dryweight : 0.;
   DryweightprAlgae = Dryweight/plantDensity;
   DryweightYieldprmeter = Dryweight/ropeDensity;
   WetweightYieldprmeter = Wetweight/ropeDensity;
}


}
