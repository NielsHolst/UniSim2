/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_LEAFUPTAKE_H
#define PESTTOX_LEAFUPTAKE_H

#include "loss_rate.h"

namespace PestTox {

class LeafUptake : public LossRate
{
public:
    LeafUptake(QString name, QObject *parent);
private:
    // Input
    QString leafType;
    double Tair, MV;
    // Methods
    double computeInstantaneous();
};

} //namespace


#endif
