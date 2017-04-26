/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ## MUSSEL GROWTH MODEL: calculate an increase of biomass for the mussel population, based in an
   intrinsic (optimum) growth rate that is scalated by current temperature and salinity at step*/

#include "mussel_growth_rate.h"

using namespace base;

namespace MusselBed {
	
MusselGrowthRate::MusselGrowthRate(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(carryingCapacity).equals(15000).help("Carrying capacity (g/m2), obtained from field observations");
    Input(mBiomass).equals(3000).help("current mussel density at step(g/m2)");
    Input(mSize).equals(1).help("average individual size in g");
    Output(mGrowth).help("maximum rate of increase considering mussel average size (% per day gr/gr)");
}

void MusselGrowthRate::initialize() {
    scales = findMany<Box>("scale");
}

void MusselGrowthRate::reset() {
    mGrowth =0.;
}

void MusselGrowthRate::update() {
    mGrowth = (0.0197*pow(mSize,-0.232));
    for (int i = 0; i < scales.size(); ++i) {
        mGrowth *= scales[i]->port("value")->value<double>();
    }

    mGrowth = mGrowth*mBiomass*(1-(mBiomass/carryingCapacity));
}


} //namespace

