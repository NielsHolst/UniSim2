/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##MUSSEL POPULATION SELF-THINNING RULE: estimates losses due to increase of individual size
   Thinning. This equation have been calculated based on field observations in the Wadden Sea.
   thinning occur when the density in numbers/m2 is too high for the current average mussel individual
   size. This model evaluates the population after it has grown for the day, and then stablish a maximum
   number of mussel/m2. The difference with the current density numbers is then eliminated from the bed.
   However as starfish predation can have a thinning effect, this thinning value is compared with starfish
   predation for the current date and the bigger one is used to determine the density loss for the mussel bed*/

#include <math.h>
#include <base/publish.h>
#include "mussel_thinning.h"


using namespace base;

namespace MusselBed {

PUBLISH(MusselThinning)

MusselThinning::MusselThinning(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(biomass).equals(1).help("current mussel density biomass g/m2");
    Input(abundance).equals(1).help("current mussel density numbers n/m2");
    Input(size).equals(1).help("current mussel size at step in g");
    Input(supply).help("biomass of mussesl consumed by starfish g/m2");
    Output(lossAbundance).help("mussel loss as numbers n/m2");
    Output(lossBiomass).help("mussel loss as biomass g/m2");
}

void MusselThinning::update() {

        double maxAbundance =(3300*pow(size,-0.87)); /* this function calculate the maximum number of mussels per m2 for the current mussel size*/
        double thinning = (maxAbundance - abundance)* size; /*this compare the max number with the current abundance and calculate the amount of mussels
                                                            that died in this step due to intraspecific competition */

        if (thinning<0 && (-thinning)>=supply){ /* Thinning is then evaluated against the amount of mussels consumed by starfish, if thinning is larger, then
                                                    thinning will determine the decrease of mussel biomass and numbers in the step instead of starfish consumed mussels*/
            lossBiomass = - thinning;
            lossAbundance = - thinning/size;
        }
        else {
            lossBiomass = supply;
            lossAbundance = supply/size;
        }
}
} //namespace
