/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "topsoil_degradation.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (TopsoilDegradation)
	
TopsoilDegradation::TopsoilDegradation(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(inflow).equals(0.);
    Input(DT50).equals(10.);          //biodegradation half-life (days)
    Input(ff).equals(0.995);      //fractionformation of a metabolite
    Input(Sp).equals(0.);         //soil porosity
    Input(Q).equals(2.1);         //value to calculate the temperature correction factor for soil biodegradation rate (this value ranges from 2-3, in this model the value of 2.1 is adopted)
    Input(Tsoil).equals(25.);      //Average air temperature in the month of pesticide application
    Input(Vrsmc).equals(0.5);     //reference soil moisture content
    Input(rf).equals(0.7);         //exponential response factor for the influence of moisture content on biodegradation rate
    Input(fw).equals(0.25);       //fraction of water in the soil
    Input(Rainfall).equals(0.);   //rainfall (mm)
    Input(threshold).equals(1.);  //rainfall even greater than 1 mm
    Input(Doseldw).equals(0.);    //dose washed off from leaves/crops
    Input(ksv).equals(0.);
    Input(frsa).equals(0.);
    Input(frsw).equals(0.);
    Input(fsr).equals(0.);
    Input(fmp).equals(0.);


    Output(concentration);
    Output(outflow);
    Output(fsdT);           //temperature correction factor
    Output(Vsmc);           //moisture content
    Output(fsm);            //moisture correction factor
    Output(ksd);            //moisture & temperature corrected biodegradation rate constant (day-1)
    Output(sdDoseaflrm);    //dose remained at the next rainfall event is taken for leaching
    Output(degradation);    //this goes to metabolite
    Output(volatilization); //amount volatilized
    Output(Dosesr);        //fraction runoff
    Output(Dmacropore);

}

void TopsoilDegradation::update() {
    const double Tref = 25.;
    double k = log(2)/DT50;
    fsdT = pow(Q,(Tsoil - Tref)/10.);
    Vsmc = (Sp > 0) ? fw/Sp : 0;
    fsm = (Vrsmc > 0) ? pow((Vsmc/Vrsmc),rf) : 0;
    ksd = k*fsdT*fsm;

    outflow = (concentration*ksd)+(ksv*(frsa+frsw)*concentration)
            +(fsr*concentration)+(fmp*concentration);
    concentration += (inflow + Doseldw) - outflow;
    degradation = concentration*ksd;
    volatilization = ksv*(frsa+frsw)*concentration;
    Dosesr = fsr*concentration;
    Dmacropore = fmp*concentration;


}

} //namespace

