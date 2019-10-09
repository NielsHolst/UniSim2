/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "eggs_laid.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (EggsLaid)
	
EggsLaid::EggsLaid(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(eggsPerFemale).equals(100.);
    Input(population).equals(0.);
    Input(sexRatio).equals(0.5);     //fraction of females in the population

    Output(totalEggs);
}

void EggsLaid::update() {
    totalEggs = sexRatio*eggsPerFemale;
}

} //namespace

