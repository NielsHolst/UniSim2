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
    help("calculates the structural dryweight of the algae from the Area produced (g)");
    Input(kA).equals(0.6).unit("g structure/dm2").help("Structural dry weight per unit area");
    Input(Cstruct).equals(0.20).unit("g C/g structure").help("Amount of structurally bound carbon");
    Input(Nstruct).equals(0.01).unit("g N/g structure").help("Amount of structurally bound nitrogen");
    Input(kdw).equals(0.0785).help("Dry weight to wet weight ratio");
    Input(A).imports("area[value]");
    Input(C).imports("C[value]");
    Input(N).imports("N[value]");
    Output(structure).unit("g structure").help("Weight of structural biomass");
    Output(carbonReserve).unit("g C").help("Weight of carbon reserve");
    Output(nitrogenReserve).unit("g N").help("Weight of nitrogen reserve");
    Output(carbonTotal).unit("g C").help("Carbon total weight");
    Output(nitrogenTotal).unit("g N").help("Nitrogen total weight");
    Output(nitrogenPct).unit("%").help("Percentage nitrogen content (by dry weight)");
    Output(CNratio).help("C/N ratio");
    Output(total).unit("g d.w.").help("Dry weight of total biomass");
    Output(totalWetWeight).unit("g w.w.").help("Wet weight of total biomass");
}

void Biomass::reset() {
   update();
}

void Biomass::update() {
   structure = kA*A;                // g structure/dm2 * dm2 = g structure
   carbonReserve   = C*structure;   // g C/g structure * g structure = g C
   nitrogenReserve = N*structure;   // g N/g structure * g structure = g N
   carbonTotal     = carbonReserve   + Cstruct*structure;
   nitrogenTotal   = nitrogenReserve + Nstruct*structure;
   CNratio = (nitrogenTotal>0.) ? carbonTotal/nitrogenTotal : 0.;
   total = structure + carbonReserve + nitrogenReserve;
   totalWetWeight = total/kdw;
   nitrogenPct = (total>0.) ? 100.*nitrogenTotal/total : 0.;
}


}
