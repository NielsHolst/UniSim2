/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "primary_distribution_soil.h"
#include <cmath>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>

using namespace base;

namespace PestTox {

PUBLISH (PrimaryDistributionSoil)
	
PrimaryDistributionSoil::PrimaryDistributionSoil(QString name, QObject *parent)
    : PrimaryDistributionBase(name, parent)
{
    Input(fractionTakenSurroundings).imports("../surroundings[fractionTaken]");
    Input(fractionTakenCrop).imports("../crop[fractionTaken]");
}

void PrimaryDistributionSoil::update() {
    double alreadyTaken = fractionTakenSurroundings + fractionTakenCrop ;
    bool withinBounds = (TestNum::geZero(alreadyTaken) && TestNum::le(alreadyTaken, 1.));
    if (!withinBounds)
        ThrowException("Already taken fraction should be inside [0;1]").
                value(alreadyTaken).context(this);

    fractionTaken = 1. - alreadyTaken;
    doseTaken = doseApplied*fractionTaken;
}

} //namespace

