/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "primary_distribution_air.h"
#include <cmath>
#include <base/exception.h>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (PrimaryDistributionAir)
	
PrimaryDistributionAir::PrimaryDistributionAir(QString name, QObject *parent)
    : PrimaryDistributionBase(name, parent)
{
    Input(boomWidth).imports("applications[boomWidth]").help("Width of spray boom (m)");
    Input(a1).help("Wind drift coefficient");
    Input(b1).help("Wind drift coefficient");
    Input(a2).help("Wind drift coefficient");
    Input(b2).help("Wind drift coefficient");
    Input(exposureZoneWidth).help("Width of exposured zone considered (m)");
}


void PrimaryDistributionAir::update() {
    // curve = a1*exp(-x/b1) + a2*exp(-x/b2)
    double areaUnderCurve = a1*b1 + a2*b2, // m; analytical integration of negative exponential
           fractionLost = areaUnderCurve/boomWidth, // 1 = m/m
           areaUnderBoom = sqrt(fieldArea)*boomWidth,  // m2; assuming a square field
           pesticideUnderBoom = areaUnderBoom*doseApplied*1e-4, // kg = m2 * kg/ha * ha/m2
           pesticideLost = pesticideUnderBoom*fractionLost, // kg
           pesticideTotal = fieldArea*doseApplied*1e-4; // kg = m2 * kg/ha * ha/m2

    if (fractionLost > 1.)
        ThrowException("Fraction lost exceeeds 1").value(fractionLost).context(this);

    doseTaken = pesticideLost; // convert to kg/ha in exposureZone
    fractionTaken = (pesticideLost > 0) ? pesticideLost/pesticideTotal : 0.;
}

} //namespace

