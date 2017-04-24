/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH GROWTH: calculates the total increment or decrease in biomass density for starfish as a function
   of food intake and temperature (respiration costs). Assimilatio function has been calculated from mesocosm experimentation
    respiration function yielded from the work by Mark Fonds, 1989 (unpublished) */

#include "starfish_demand.h"

using namespace UniSim;

namespace mussel_bed {

StarfishDemand::StarfishDemand(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("stSize",&stSize, 5., this, "starfish individual size at step");
    new Parameter<double>("stBiomass", &stBiomass, 10., this, "current starfish population biomass (g/m2)");
    new Parameter<double>("temperature", &temperature, 13., this, "current temperature");
    new Variable<double>("netDemand", &netDemand, this, "food demand to spend in growth g flesh/m2");
    new Variable<double>("Respiration", &Respiration, this, "food needed to cover respiration/maintenance costs g flesh/m2");
}


void StarfishDemand::reset() {
    netDemand = 1.;
    Respiration = 0.006;
}

void StarfishDemand::update() {

    netDemand = (0.11153368 - 0.021521*log(stSize))*stBiomass; /* Here the food demand required for growth under optimal conditions is calculated, this demand does not
                                                  include respiration costs, is all for growth*/
    Respiration = (0.0004576*exp(0.2157*temperature))*stBiomass; /*Here the demand of food to cover for body maintenance costs */
}

} //namespace
