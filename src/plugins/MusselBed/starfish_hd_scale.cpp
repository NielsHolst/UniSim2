/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH FEEDING HYDRODYNAMICS SCALING: this function scales starfish mussel biomass demand to the location Hydrodynamic Regime.
** Function yielded from Antonio Agüera, et al. 2014
*/

#include <base/publish.h>
#include "starfish_hd_scale.h"

using namespace base;

namespace MusselBed {

PUBLISH(StarfishHdScale)

StarfishHdScale::StarfishHdScale(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(vmax).equals(70).help("maximum flow velocity at location (cm/s)");
    Input(mBiomass).equals(1).help("current mussel biomass, mussel biomass reduce HD's effect");
    Output(scale).help("scaling factor for current hydrodynamics with a density of 1000g/mussel");
    Output(value).help("scaling factor for search rate at current mBiomass and flow");
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
