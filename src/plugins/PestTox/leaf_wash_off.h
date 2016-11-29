/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LEAF_WASH_OFF_H
#define LEAF_WASH_OFF_H

#include "loss_rate.h"

namespace PestTox {

class LeafWashOff : public LossRate
{
public:
    LeafWashOff(QString name, QObject *parent);

private:
    // Input
    double rainfall, washOffCoefficient;

    // Methods
    double computeInstantaneous();
};

} //namespace


#endif
