/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "surface_water_degradation_final.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (surfacewaterdegradationfinal )
	
surfacewaterdegradationfinal ::surfacewaterdegradationfinal (QString name, QObject *parent)
	: Box(name, parent)
{

    Input(inflow).equals(0.);
    Input(k).equals(0.);          //biodegradation rate constant (day-1)
    Input(ff).equals(0.995);      //fractionformation of a metabolite
    Input(Tref).equals(25.);      //Reference temperature at which t½ has been determined  in deg C
    Input(Q).equals(2.1);         //value to calculate the temperature correction factor for soil biodegradation rate (this value ranges from 2-3, in this model the value of 2.1 is adopted)
    Input(Tair).equals(25.);      //Average air temperature in the month of pesticide application

    Output(concentration);
    Output(outflow);
    Output(Tw);             //actual water temp in deg C
    Output(fsdT);           //temperature correction factor    
    Output(kwd);            //temperature corrected biodegradation rate constant (day-1)

}

void surfacewaterdegradationfinal ::reset() {
    concentration = 0.;
    outflow = 0.;
    Tw = 0.;
    kwd = 0.;
    fsdT = 0.;

}

void surfacewaterdegradationfinal ::update() {
    Tw = 1.05*Tair - 1.5;
    fsdT = std::pow(Q,(Tw - Tref)/10.);
    kwd = k*fsdT;
    outflow = concentration*ff*kwd;
    concentration += inflow - outflow;

}

} //namespace

