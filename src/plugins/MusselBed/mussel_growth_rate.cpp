/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ## MUSSEL GROWTH MODEL: calculate an increase of biomass for the mussel population, based in an
   intrinsic (optimum) growth rate that is scalated by current temperature and salinity at step*/
#include <base/publish.h>
#include "mussel_growth_rate.h"

using namespace base;

namespace MusselBed {

PUBLISH(MusselGrowthRate)

MusselGrowthRate::MusselGrowthRate(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(carryingCapacity).equals(15000).help("Carrying capacity (g/m2), obtained from field observations");
    Input(biomass).equals(3000).help("current mussel density at step(g/m2)");
    Input(size).equals(1).help("average individual size in g");
    Output(value).help("maximum rate of increase considering mussel average size (% per day gr/gr)");
}

void MusselGrowthRate::initialize() {
    scales = findMany<Box>("scale");
}

void MusselGrowthRate::reset() {
    value =0.;
}

void MusselGrowthRate::update() {
    value = (0.0197*pow(size,-0.232));
    for (int i = 0; i < scales.size(); ++i) {
        value *= scales[i]->port("value")->value<double>();
    }

    value = value*biomass*(1-(biomass/carryingCapacity));
}


} //namespace

