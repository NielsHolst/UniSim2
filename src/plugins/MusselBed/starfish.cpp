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

using namespace base;

namespace MusselBed {

Starfish::Starfish(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(stinBiomass).help("Starfish density at t0 (g/m2");
    Input(stinAbundance).help("Starfish density at t0 in numbers");
    Input(supply).help("consumed food under current environmental condtions this supply have to be used for respiration and growth");
    Input(respiration).help("food necessary to cover for maintenance costs");
    Output(stAbundance).help("starfish density in numbers");
    Output(stBiomass).help("current step starfish density (g/m2)");
    Output(stSize).help("current average individual size (g)");
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
    if (growthrate<0 && fabs(growthrate)>stBiomass) {  /*growth rate can be negative, although respiration costs tend to zero as biomass decrease, to ensure*/
        stBiomass = 0;                          /*apropiate model behavior, this function avoid negative densities.*/
    }
    else {
        stBiomass = growthrate+stBiomass;
    }

    stSize = stBiomass/stAbundance; /*abundance as number of individuals is constant, this model does not incorporate predator mortality or migration, etc. this
                                    part updates individual size considering the increase in biomass.*/

}

} //namespace

