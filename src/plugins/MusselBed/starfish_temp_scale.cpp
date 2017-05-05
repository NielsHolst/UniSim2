/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH SEARCH TEMPERATURE SCALING: this function scales starfish mussel biomass demand to the current
   temperature at step. Function yielded from Antonio Agüera, et al. 2012. Winter feeding activity of the common starfish (Asterias rubens L.):
   The role of temperature and shading, Journal of Sea Research, Volume 72, August 2012, Pages 106-112,
   (http://www.sciencedirect.com/science/article/pii/S1385110112000159)*/

#include <base/publish.h>
#include "starfish_temp_scale.h"

using namespace base;

namespace MusselBed {

PUBLISH(StarfishTempScale)

StarfishTempScale::StarfishTempScale(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(temperature).equals(11).help("current temperature degree celsius");
    Output(value).help("scaling factor for feeding rate");
}

void StarfishTempScale::reset() {
    value = 1;
}

void StarfishTempScale::update() {
    const double a = 0.52777,
                 b = 3.96623;
    value =(exp(a*temperature - b)/(1 + exp(a*temperature - b)));
}

} //namespace
