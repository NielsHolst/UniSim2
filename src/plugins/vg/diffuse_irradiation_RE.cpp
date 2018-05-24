/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "diffuse_irradiation_RE.h"
#include "general.h"
using namespace base;

// From Kropff & Laar (1993), pp. 235-236

namespace vg {

PUBLISH(DiffuseIrradiationRE)

DiffuseIrradiationRE::DiffuseIrradiationRE(QString name, QObject *parent)
    : DiffuseIrradiationBase(name, parent)
{
    help("computes diffuse fraction of global irradiation");
    Input(temperature).imports("outdoors[temperature]");
    Input(rh).imports("outdoors[rh]");
    Input(sinb).imports("calendar[sinb]");
    Output(value).help("Diffuse irradiation (W/m2)");
    Output(fraction).help("Diffuse fraction of global irradiation [0;1]");
    Output(eq).help("Equation number used");
}

void DiffuseIrradiationRE::reset() {
    update();
}

void DiffuseIrradiationRE::update() {
    updateClearness();
    if (globalIrradiation == 0.) {
        fraction = value = 0.;
        eq = 0;
    }
    else if (clearness <= 0.3) {
        fraction = 1.020 - 0.254*clearness + 0.0123*sinb;
        eq = 1;
    }
    else if (clearness < 0.78) {
        fraction = 1.400 - 1.749*clearness + 0.177*sinb;
        eq = 2;
    }
    else {
        fraction = 0.486*clearness - 0.182*sinb;
        eq = 3;
    }

//    else if (clearness <= 0.3)
//        fraction = 1. - 0.232*clearness
//                      + 0.0239*sinb
//                      - 0.000682*temperature
//                      + 0.019*rh;
//    else if (clearness <= 0.78)
//        fraction = 1.329 - 1.716*clearness
//                         + 0.267*sinb
//                         - 0.00357*temperature
//                         + 0.106*rh;
//    else
//        fraction = 0.426*clearness
//                   + 0.256*sinb
//                   - 0.00349*temperature
//                   + 0.0734*rh;
    fraction = minmax(0., fraction, 1.);
    value = fraction*globalIrradiation;
}

} //namespace

