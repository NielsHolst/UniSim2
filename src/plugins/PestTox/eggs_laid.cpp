/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "eggs_laid.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (eggslaid)
	
eggslaid::eggslaid(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(eggsPerFemale).equals(100.);
    Input(population).equals(0.);
    Input(sexRatio).equals(0.5);     //fraction of females in the population

    Output(totalEggs);
}

void eggslaid::reset() {

    totalEggs = 0.;
}

void eggslaid::update() {

    //totalEggs = (population >2) ? sexRatio*eggsPerFemale : 0;

    totalEggs = sexRatio*eggsPerFemale;
}

} //namespace

