/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "surface_water_degradation.h"
#include <cmath>
#include <base/publish.h>

using namespace base;
using namespace std;

namespace PestTox {

PUBLISH (SurfaceWaterDegradation )
	
SurfaceWaterDegradation ::SurfaceWaterDegradation (QString name, QObject *parent)
	: Box(name, parent)
{

    Input(inflow).equals(0.);
    Input(DT50).equals(4.5); //biodegradation half-life (days)
    Input(ff).equals(0.995);      //fractionformation of a metabolite
    Input(Q).equals(2.1);         //value to calculate the temperature correction factor for soil biodegradation rate (this value ranges from 2-3, in this model the value of 2.1 is adopted)
    Input(Tw);             //actual water temp in deg C

    Output(concentration);
    Output(outflow);
    Output(fsdT);           //temperature correction factor    
    Output(kwd);            //temperature corrected biodegradation rate constant (day-1)

}

void SurfaceWaterDegradation ::update() {
    const double Tref = 25.;
    double k = log(2)/DT50;
    fsdT = pow(Q, (Tw - Tref)/10.);
    kwd = k*fsdT;
    outflow = concentration*ff*kwd;
    concentration += inflow - outflow;

}

} //namespace

