/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH FEEDING HYDRODYNAMICS SCALING: this function scales starfish mussel biomass demand to the location Hydrodynamic Regime.
** Function yielded from Antonio Agüera, et al. 2014
*/

#include "starfish_hd_scale.h"
#include <math.h>
using namespace UniSim;

namespace mussel_bed {

StarfishHdScale::StarfishHdScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("vmax", &vmax, 70., this, "maximum flow velocity at location (cm/s)");
    new Parameter<double>("mBiomass", &mBiomass, 1., this, "current mussel biomass, mussel biomass reduce HD's effect");
    new Variable<double>("scale", &scale, this, "scaling factor for current hydrodynamics with a density of 1000g/mussel");
    new Variable<double>("value", &value, this, "scaling factor for search rate at current mBiomass and flow");
}

void StarfishHdScale::reset() {

    double a=3.87297;
    double b=-0.09605;

    for (int t=0; t<=180; t++) {
        double x = exp(a+(b*vmax*sin(M_PI*t/360)));
        scale += x/(1+x);
        }
        scale = (scale/180);

    }



void StarfishHdScale::update() {

    if (mBiomass<=5000)
        value=scale+(((mBiomass-1000)/5000)*(1-scale));
    else
        value=1;
}
}
//namespace
