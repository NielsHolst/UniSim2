/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH(crop)
	
crop::crop(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(day).equals(1);            //sowing time (day)
    Input(seeds).equals(1.);      //proportion of seeds (fraction)

    Output(seedsSown);
}

void crop::reset() {
    daysPassed = 0;
    seedsSown = 0;
}

void crop::update() {
    ++daysPassed;
    seedsSown = (daysPassed == day) ? seeds : 0.;
}

} //namespace

