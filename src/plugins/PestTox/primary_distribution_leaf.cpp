/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "primary_distribution_leaf.h"
#include <base/publish.h>
#include <base/vector_op.h>

using namespace base;

namespace PestTox {

PUBLISH (PrimaryDistributionLeaf)
	
PrimaryDistributionLeaf::PrimaryDistributionLeaf(QString name, QObject *parent)
    : PrimaryDistributionBase(name, parent)
{
    Input(fractionTakenAir).imports("../surroundings[fractionTaken]");
    Input(stageContent).imports("crop/*{Stage}[content]");
    Input(fractionsByCropStage);
}

void PrimaryDistributionLeaf::update() {
    double fractionLeft = 1. - fractionTakenAir;
    fractionTaken = vector_op::sumOfProducts(fractionsByCropStage, stageContent)*fractionLeft;
    doseTaken = doseApplied*fractionTaken;
}

} //namespace

