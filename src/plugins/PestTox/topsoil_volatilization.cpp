/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <cmath>
#include <base/publish.h>
#include "general.h"
#include "topsoil_volatilization.h"

using namespace base;

namespace PestTox {

PUBLISH (TopsoilVolatilization)
	
TopsoilVolatilization::TopsoilVolatilization(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(Tair).imports("weather[Tavg]");           // degrees celsius
    Input(MV).equals(140.83);         //molar volume; density 1.704 g/cm3, molar mass 169.07 g/mol
    Input(BLair).equals(0.00475);     // m or 4.75 mm as per Mackay (2001)
    Input(Pd).equals(0.01);           //penetration depth of 1 cm = 0.01 m
    Input(fa).equals(0.25);           //fraction of air in the soil
    Input(fw).equals(0.25);           //fraction of water in the soil
    Input(Pair).equals(1.);           //atm
    Input(farea).equals(100.);        //m2
    Input(Za).equals(0.);             //mol/m3Pa
    Input(Zw).equals(0.);             //mol/m3Pa
    Input(VZ).equals(0.);
    Input(MW).equals(169.07);         // molecular weight or molar mass of the insecticide in g/mol
    Input(viscosity).equals(0.);      //viscosity of water (centipoise),between 0 - 50 deg C

    Output(Ba);
    Output(Dbl);    
    Output(Bea);
    Output(Bew);               //effective diffusivity (m2/s)
    Output(Da);                //mol/m3day
    Output(Dw);                //mol/m3day
    Output(Dv);                //mol/m3day
    Output(ksv);               //rate constant of pesticide evaporation from the soil (day-1)
    Output(Bw);                //molecular diffusivity of selected pesticide in water (m2/s)
    Output(Dpl);               //diffusion path length (m)
    Output(ke);                //transport velocity through boundary layer (m/s)
}

void TopsoilVolatilization::update() {

    Ba = 1./10000. * 1./1000. * (pow((Tair + 273.15), 1.75)*sqrt(((1./MWair) + (1./MW))) / (Pair*pow((pow(MVair, 1./3.) + pow(MV, 1./3.)), 2)));
    ke = Ba / BLair;
    Dbl = 3600. * 24. * ke * farea * Za;
    Bea = Ba*pow(fa, 10./3.)/pow((fa+fw), 2);
    Bw = (1./10000.)*0.0001326/(pow(viscosity, 1.14)* pow(MV, 0.589));
    Bew = Bw*pow(fw, 10./3.)/pow((fa+fw), 2);
    Dpl = Pd / 2.;
    Da = 3600. * 24. * Bea * farea * Za / Dpl;
    Dw = 3600. * 24. * Bew * farea * Zw / Dpl;
    Dv = 1./((1./Dbl)+(1./(Da + Dw)));
    ksv = Dv/VZ;

}

} //namespace

