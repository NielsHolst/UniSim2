#include <base/publish.h>
#include "Rate_of_change_in_carbon_reserves.h"

using namespace base;

namespace saccharina {

PUBLISH(Rateofchangeincarbonreserves)

Rateofchangeincarbonreserves::Rateofchangeincarbonreserves(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the rate of change in carbon reserves (Eq 7)");
    Input(kA).equals(0.6).help("Structural dry weight per unit area");
    Input(Gphotosynthesis).equals(0.00122).help("Gross photosynthesis");
    Input(Cexudation).equals(0.5).help("Carbon Exudation");
    Input(Respiration).equals(0.0005429).help("Respiration");
    Input(C).equals(0.01).help("Carbon reserves");
    Input(Cstruct).equals(0.2).help("Amount of structurally bound carbon");
    Input(u).equals(0.18).help("Specific growth rate");
    Output(carbonreservesrate).help("Rate of change in carbon reserves");
}

void Rateofchangeincarbonreserves::reset() {
   update();
}

void Rateofchangeincarbonreserves::update() {
    carbonreservesrate = pow(kA,-1) * (Gphotosynthesis * (1 - Cexudation) - Respiration) - (C + Cstruct) * u;
}
}
