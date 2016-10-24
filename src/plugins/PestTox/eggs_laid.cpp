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
    Input(population).equals(100.);
    Input(sexRatio).equals(0.6);     //per cent of eggs, tadpoles, ....adult frogs die per day
    Output(totaleggs);
}

void eggslaid::reset() {
    totaleggs = 0.;
}

void eggslaid::update() {
    totaleggs = (population >2) ? sexRatio*eggsPerFemale*population : 0;
}

} //namespace

