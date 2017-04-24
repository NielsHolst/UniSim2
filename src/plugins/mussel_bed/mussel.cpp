/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.*/

/* ## MUSSEL POPULATION MODEL: actualize mussel population density at each step, considering the losses caused by thinning and predation
   and an environmentally scalated growth rate.*/
#include "mussel.h"

using namespace UniSim;

namespace mussel_bed {
	
Mussel::Mussel(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("minBiomass", &minBiomass, 3000., this, "Observed density at t0 as g/m2");
    new Parameter<double>("minAbundance", &minAbundance, 1000., this, "Observed density at t0 as numbers/m2");
    new Parameter<double>("lsBiomass", &lsBiomass, 0., this, "losses in mussel biomass as g/m2");
    new Parameter<double>("lsAbundance", &lsAbundance, 0., this, "losses in mussel numbers as g/m2");
    new Parameter<double>("mGrowth", &mGrowth, 0.3, this, "mussel growth rate (only in biomass)");
    new Parameter<int>("day",&day, 0., this, "current step");
    new Variable<double>("mAbundance",&mAbundance,this,"current density at step, numbers/m2");
    new Variable<double>("mBiomass", &mBiomass, this, "current density at step, g/m2");
    new Variable<double>("mSize", &mSize, this, "mean mussel size at step (g)");
    new Variable<double>("LD",&LD,this,"remaining percentage mussel biomass");
    new Variable<double>("LD50",&LD50,this,"day 50% losses reached");

}

void Mussel::reset() {
    mBiomass = minBiomass;
    mAbundance = minAbundance;
    mSize = minBiomass/minAbundance;
    LD = 0;
    LD50 = 61;

}

void Mussel::update() {

    mBiomass = mBiomass + mGrowth - lsBiomass;
    mAbundance = mAbundance - lsAbundance;

    if (mBiomass <0) /* to avoid problems if density gets below 0, temporal fix*/
        mBiomass=0;

    mSize = mBiomass/mAbundance;

    LD = mBiomass/minBiomass;

    if (LD <= 0.5 && day<LD50)
        LD50 = day;
}
} //namespace

