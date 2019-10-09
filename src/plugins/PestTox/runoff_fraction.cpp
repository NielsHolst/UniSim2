/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "runoff_fraction.h"
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (RunoffFraction)
	
RunoffFraction::RunoffFraction(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(Q).equals(5.);
    Input(P).equals(1.);      //average  daily rainfall per rainfall event in a given month (mm)

    Output(frr);
}

void RunoffFraction::update() {
    frr = (P > 0) ? Q/P : 0;
}

} //namespace

