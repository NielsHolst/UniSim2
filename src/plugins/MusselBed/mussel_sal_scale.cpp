/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##SALINITY SCALE FOR MUSSEL GROWTH: scale mussel growth with the effect of the current step daily average salinity.
   Results are yielded from Almada-Villela, 1984*/

#include <base/publish.h>
#include "mussel_sal_scale.h"

using namespace base;

namespace MusselBed {

PUBLISH(MusselSalScale)

MusselSalScale::MusselSalScale(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(a).equals(0.498).help("Regression coefficient");
    Input(b).equals(-11.798).help("Regression coefficient");
    Input(salinity).equals(24.).help("mean salinity");
    Output(value).help("mussel growth scaling factor");
}

void MusselSalScale::reset() {
    update();
}

void MusselSalScale::update() {
//    value = exp(0.24113*salinity-3.484087)/(1+exp(0.24113*salinity-3.484087));
    value = exp(a*salinity + b)/(1.+exp(a*salinity + b));
}

} //namespace

