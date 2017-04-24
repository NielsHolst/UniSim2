/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##SALINITY SCALE FOR MUSSEL GROWTH: scale mussel growth with the effect of the current step daily average salinity.
   Results are yielded from Almada-Villela, 1984*/

#include "mussel_sal_scale.h"

using namespace UniSim;

namespace mussel_bed {

MusselSalScale::MusselSalScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("smean", &smean, 24., this, "mean salinity");
    new Variable<double>("value", &value, this, "mussel growth scaling factor");
}

void MusselSalScale::reset() {
    value = 1;

}

void MusselSalScale::update() {
    value = exp(0.24113*smean-3.484087)/(1+exp(0.24113*smean-3.484087));
}
} //namespace

