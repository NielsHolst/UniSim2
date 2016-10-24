/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "zvalues.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (zvalues)
	
zvalues::zvalues(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(MW).equals(169.07);         //molar mass of the pesticide in g/mol
    Input(Pd).equals(0.01);           //penetration depth of 1 cm = 0.01 m
    Input(VP).equals(1.31e-5);        //Pa at 25 degrees celsius
    Input(Psol).equals(12.);          //solubility g/L
    Input(foc).equals(0.19);
    Input(pb).equals(1.1);            // 1.1 kg/L which is equivalent to 1.1 g/cm3 or 1100 kg/m3
    Input(fa).equals(0.25);           //fraction of air in the soil
    Input(fw).equals(0.25);           //fraction of water in the soil
    Input(fs).equals(0.5);            //fraction of solids in the soil
    Input(KocpH).equals(5.678e-6);    //The acid dissociation constants for glyphosate are pKa1 0.8 (1st phosphonic), pKa2 2.3 (carboxylate), pKa3 6.0 (2nd phosphonic), and pKa4 11.0 (amine).
    Input(TrefVP).equals(25.);        //reference temperature of vapour pressure (degrees celsius)
    Input(Tsa).equals(30.);           //Average soil air temperature in the month of pesticide application  (degrees celsius)
    Input(R).equals(8.3145);          //J mol-1 K-1
    Input(Sp).equals(0.5);            //soil porosity
    Input(farea).equals(100.);


    Output(Za);                //mol m-3 Pa-1
    Output(Zw);                //mol m-3 Pa-1
    Output(Zs);                //mol m-3 Pa-1
    Output(VPTadap);
    Output(Vs);                //volume of solids in the soil volume susceptible for volatilization (m3)
    Output(Vw);                //volume of water in the soil volume susceptible for volatilization (m3)
    Output(Va);                //volume of air in the soil volume susceptible for volatilization (m3)
    Output(VZ);

}

void zvalues::reset() {
    Za = 0.;
    Zw = 0.;
    Zs = 0.;
    Vs = 0.;
    Vw = 0.;
    Va = 0.;
    VPTadap = 0.;
    VZ = 0.;

}

void zvalues::update() {

    VPTadap = VP * exp((1./10.)*(Tsa - TrefVP));
    Zw = 1000.*Psol/(VPTadap * MW);
    Za = 1./(R*(Tsa + 273.15));
    Zs = (KocpH *foc * pb * Zw) / (1. - Sp);
    Vs = Pd*fs*farea;
    Vw = Pd*fw*farea;
    Va = Pd*fa*farea;
    VZ = Va*Za + Vw*Zw + Vs*Zs;


}

} //namespace

