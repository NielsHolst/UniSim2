/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "diffuse_irradiation_RE.h"

using namespace base;
using namespace phys_math;

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

    fraction = minmax(0., fraction, 1.);
    value = fraction*globalIrradiation;
}

} //namespace

