/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH FEEDING SALINITY SCALING: this function scales starfish mussel biomass demand to the current
   salinity at step. This expression is based in field observations in mesocosm experiment at the laboratory*/

#include "starfish_sal_scale.h"
#include <math.h>
using namespace base;

namespace MusselBed {

StarfishSalScale::StarfishSalScale(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(smean).equals(28).help("current salinity");
    Output(value).help("scaling factor for feeding rate");
}

void StarfishSalScale::reset() {
    value = 1;
}

void StarfishSalScale::update() {
    const double a = 16.83070,
                 b =  0.74301;
    value = exp(-a + b*smean)/(1+exp(-a + b*smean));
}

} //namespace

