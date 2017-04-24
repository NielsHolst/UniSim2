/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ## STARFISH POPULATION MODEL: actualize starfish population density at each step, considering environmentally scalated supply and population maintenance costs.
      Due to how the FunctionalResponseGB plugin works, this function uses netSupply that includes total consumed flesh. When consumed flesh is 0 or smaller than
      respiration costs, starfish shrink at rate determined by respiration*/

#include "starfish.h"
#include "cmath"

using namespace UniSim;

namespace mussel_bed {

Starfish::Starfish(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("stinBiomass", &stinBiomass, 0., this, "Starfish density at t0 (g/m2");
    new Parameter<double>("stinAbundance", &stinAbundance, 0, this, "Starfish density at t0 in numbers");
    new Parameter<double>("supply", &supply, 0, this, "consumed food under current environmental condtions this supply have to be used for respiration and growth");
    new Parameter<double>("respiration",&respiration, 0., this, "food necessary to cover for maintenance costs");
    new Variable<double>("stAbundance", &stAbundance, this, "starfish density in numbers");
    new Variable<double>("stBiomass", &stBiomass, this, "current step starfish density (g/m2)");
    new Variable<double>("stSize", &stSize, this, "current average individual size (g)");
}
void Starfish::reset() {
    stBiomass = stinBiomass;
    stAbundance = stinAbundance;
    stSize=stinBiomass/stinAbundance;
}

void Starfish::update() {
    double growthrate = 0.848*((supply) - respiration); /*this function calculates the growth rate as a conversion factor of the food available after covering
                                                        maintenance costs. Growth rate can be negative if supply doesn't cover maintenance costs, in that case
                                                        starfish population shrink in biomass not in numbers*/
    if (growthrate<0 && abs(growthrate)>stBiomass) {  /*growth rate can be negative, although respiration costs tend to zero as biomass decrease, to ensure*/
        stBiomass = 0;                          /*apropiate model behavior, this function avoid negative densities.*/
    }
    else {
        stBiomass = growthrate+stBiomass;
    }

    stSize = stBiomass/stAbundance; /*abundance as number of individuals is constant, this model does not incorporate predator mortality or migration, etc. this
                                    part updates individual size considering the increase in biomass.*/

}

} //namespace

