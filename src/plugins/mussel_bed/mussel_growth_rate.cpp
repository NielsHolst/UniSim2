/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ## MUSSEL GROWTH MODEL: calculate an increase of biomass for the mussel population, based in an
   intrinsic (optimum) growth rate that is scalated by current temperature and salinity at step*/

#include "mussel_growth_rate.h"

using namespace UniSim;

namespace mussel_bed {
	
MusselGrowthRate::MusselGrowthRate(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("carryingCapacity", &carryingCapacity, 15000., this, "Carrying capacity (g/m2), obtained from field observations");
    new Parameter<double>("mBiomass", &mBiomass, 3000., this, "current mussel density at step(g/m2)");
    new Parameter<double>("mSize", &mSize, 1., this, "average individual size in g");
    new Variable<double>("mGrowth", &mGrowth, this, "maximum rate of increase considering mussel average size (% per day gr/gr)");
}

void MusselGrowthRate::initialize() {
    scales = seekChildren<Model*>("scale");
}

void MusselGrowthRate::reset() {
    mGrowth =0.;
}

void MusselGrowthRate::update() {
    mGrowth = (0.0197*pow(mSize,-0.232));
    for (int i = 0; i < scales.size(); ++i) {
        mGrowth *= scales[i]->pullValue<double>("value");
    }

    mGrowth = mGrowth*mBiomass*(1-(mBiomass/carryingCapacity));
}


} //namespace

