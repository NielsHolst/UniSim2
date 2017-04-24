/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH SEARCH TEMPERATURE SCALING: this function scales starfish mussel biomass demand to the current
   temperature at step. Function yielded from Antonio Agüera, et al. 2012. Winter feeding activity of the common starfish (Asterias rubens L.):
   The role of temperature and shading, Journal of Sea Research, Volume 72, August 2012, Pages 106-112,
   (http://www.sciencedirect.com/science/article/pii/S1385110112000159)*/

#include "starfish_temp_scale.h"
#include <math.h>
using namespace UniSim;

namespace mussel_bed {

StarfishTempScale::StarfishTempScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("temperature", &temperature, 11., this, "current temperature degree celsius");
    new Variable<double>("value", &value, this, "scaling factor for feeding rate");
}

void StarfishTempScale::reset() {
    value = 1;
}

void StarfishTempScale::update() {
    value =(exp(0.52777*temperature - 3.96623)/(1+exp(0.52777*temperature - 3.96623)));
    }

} //namespace
