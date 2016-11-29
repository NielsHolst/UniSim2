/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PRIMARY_DISTRIBUTION_LEAF_H
#define PRIMARY_DISTRIBUTION_LEAF_H

#include "primary_distribution_base.h"

namespace PestTox {

class PrimaryDistributionLeaf : public PrimaryDistributionBase
{
public:
    PrimaryDistributionLeaf(QString name, QObject *parent);
    void update();

private:
    // Input
    double fractionTakenAir;
    QVector<double> fractionsByCropStage, stageContent;
};

} //namespace


#endif
