/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "topsoil_in_soil.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (TopsoilInSoil)
	
TopsoilInSoil::TopsoilInSoil(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(Z).equals(0.);           //mol/m3Pa
    Input(V).equals(0.);           //volume susceptible for volatilization (m3)
    Input(VZ).equals(0.);

    Output(fr);
}

void TopsoilInSoil::reset() {
    update();
}


void TopsoilInSoil::update() {
    fr = (VZ > 0) ? V*Z/VZ : 0.;
}

} //namespace

