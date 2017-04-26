/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH GROWTH: calculates the total increment or decrease in biomass density for starfish as a function
   of food intake and temperature (respiration costs). Assimilatio function has been calculated from mesocosm experimentation
    respiration function yielded from the work by Mark Fonds, 1989 (unpublished) */

#include "starfish_demand.h"

using namespace base;

namespace MusselBed {

StarfishDemand::StarfishDemand(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(stSize).equals(5).help("starfish individual size at step");
    Input(stBiomass).equals(10).help("current starfish population biomass (g/m2)");
    Input(temperature).equals(13).help("current temperature");
    Output(netDemand).help("food demand to spend in growth g flesh/m2");
    Output(Respiration).help("food needed to cover respiration/maintenance costs g flesh/m2");
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
