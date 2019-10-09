/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    Input(a1).help("Wind drift coefficient");
    Input(b1).help("Wind drift coefficient");
    Input(a2).help("Wind drift coefficient");
    Input(b2).help("Wind drift coefficient");
    Input(x).equals(1.).help("Distance off field edge");
}


void PrimaryDistributionAir::update() {
    double fractionLost = (a1/100.)*exp(-x/b1) + (a2/100.)*exp(-x/b2), // 1 = m/m
           pesticideLost = doseApplied*fractionLost; // g/ha * 1

    if (fractionLost > 1.)
        ThrowException("Fraction lost exceeeds 1").value(fractionLost).context(this);

    doseTaken = pesticideLost; // g/ha (convert accordingly in exposureZone)
    fractionTaken = fractionLost;
}

} //namespace

