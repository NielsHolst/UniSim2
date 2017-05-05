/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.*/

/* ## MUSSEL POPULATION MODEL: actualize mussel population density at each step, considering the losses caused by thinning and predation
   and an environmentally scalated growth rate.*/
#include <base/publish.h>
#include "mussel.h"

using namespace base;

namespace MusselBed {

PUBLISH(Mussel)

Mussel::Mussel(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(initBiomass).equals(3000).help("Observed density at t0 as g/m2");
    Input(initAbundance).equals(1000).help("Observed density at t0 as numbers/m2");
    Input(lossBiomass).help("losses in mussel biomass as g/m2");
    Input(lossAbundance).help("losses in mussel numbers as g/m2");
    Input(growth).equals(0.3).help("mussel growth rate (only in biomass)");
    Input(day).help("current step");
    Output(abundance).help("current density at step, numbers/m2");
    Output(biomass).help("current density at step, g/m2");
    Output(size).help("mean mussel size at step (g)");
    Output(LD).help("remaining percentage mussel biomass");
    Output(LD50).help("day 50% losses reached");

}

void Mussel::reset() {
    biomass = initBiomass;
    abundance = initAbundance;
    size = initBiomass/initAbundance;
    LD = 0;
    LD50 = 61;

}

void Mussel::update() {

    biomass = biomass + growth - lossBiomass;
    abundance = abundance - lossAbundance;

    if (biomass <0) /* to avoid problems if density gets below 0, temporal fix*/
        biomass=0;

    size = biomass/abundance;

    LD = biomass/initBiomass;

    if (LD <= 0.5 && day<LD50)
        LD50 = day;
}
} //namespace

