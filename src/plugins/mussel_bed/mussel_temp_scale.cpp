/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* MUSSEL GROWTH SCALING: Scale maximum growth rate by current step temperature,
   this scaling function is based in the data from Almeda-Villela et al., 1982, Fig 2. */

#include "mussel_temp_scale.h"
#include <math.h>
using namespace UniSim;

namespace mussel_bed {
	
MusselTempScale::MusselTempScale(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("temperature", &temperature, 12., this, "current step temperature in degree Celsius");
    new Variable<double>("value", &value, this, "scaling parameter at current temperature");
}

void MusselTempScale::reset() {
    value = 1;
}

void MusselTempScale::update() {

    value =(exp(0.40831*temperature - 4.15734)/(1+exp(0.40831*temperature - 4.15734)));
    }


}//namespace


