/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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

