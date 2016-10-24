/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "topsoil_dose_in_soil_solids_air_and_water.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (topsoildoseinsoilsolidsairandwater)
	
topsoildoseinsoilsolidsairandwater::topsoildoseinsoilsolidsairandwater(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(Za).equals(0.);           //mol/m3Pa
    Input(Zw).equals(0.);           //mol/m3Pa
    Input(Zs).equals(0.);
    Input(Vs).equals(0.);           //volume of solids in the soil volume susceptible for volatilization (m3)
    Input(Vw).equals(0.);           //volume of water in the soil volume susceptible for volatilization (m3)
    Input(Va).equals(0.);           //volume of air in the soil volume susceptible for volatilization (m3)
    Input(VZ).equals(0.);

    Output(frss);
    Output(frsw);
    Output(frsa);
}

void topsoildoseinsoilsolidsairandwater::reset() {

    frss = 0.;
    frsw = 0.;
    frsa = 0.;
}


void topsoildoseinsoilsolidsairandwater::update() {

    frss = (Vs*Zs)/VZ;      //Fraction pesticide in soil solids; ((Vs*Zs)/VZ)
    frsw = (Vw*Zw)/VZ;      //Fraction pesticide in soil water
    frsa = (Va*Za)/VZ;      //Fraction pesticide in soil air; ((Va*Za)/VZ)

}

} //namespace

