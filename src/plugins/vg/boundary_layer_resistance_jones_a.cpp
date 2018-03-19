/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/publish.h>
#include "boundary_layer_resistance_jones_a.h"

using namespace std;
using namespace base;

namespace vg {

PUBLISH(BoundaryLayerResistanceJonesA)

BoundaryLayerResistanceJonesA::BoundaryLayerResistanceJonesA(QString name, QObject *parent)
    : BoundaryLayerResistanceBase(name, parent)
{
    Class(BoundaryLayerResistanceJonesA);
    help("computes boundary layer resistance from Jones (1992, eq. 3.31)");
}

void BoundaryLayerResistanceJonesA::setRbH2O() {
    double gbH2O = 6.62*sqrt(leafWindSpeed/leafDimension),
           rbH2Olaminar = 1000./gbH2O,
           fTurbulence = 1.5;

    rbH2O = rbH2Olaminar/fTurbulence;
}

} //namespace

