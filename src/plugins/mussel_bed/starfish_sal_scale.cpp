/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH FEEDING SALINITY SCALING: this function scales starfish mussel biomass demand to the current
   salinity at step. This expression is based in field observations in mesocosm experiment at the laboratory*/

#include "starfish_sal_scale.h"
#include <math.h>
using namespace UniSim;

namespace mussel_bed {

StarfishSalScale::StarfishSalScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("smean", &smean, 28., this, "current temperature degree celsius");
    new Variable<double>("value", &value, this, "scaling factor for feeding rate");
}

void StarfishSalScale::reset() {
    value = 1;
}

void StarfishSalScale::update() {
    value = ((exp(-16.83070+0.74301*smean)/(1+exp(-16.83070+0.74301*smean))));
    }

} //namespace

