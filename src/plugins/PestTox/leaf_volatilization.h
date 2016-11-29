/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_LEAFVOLATILIZATION_H
#define PESTTOX_LEAFVOLATILIZATION_H

#include "loss_rate.h"

namespace PestTox {

class LeafVolatilization : public LossRate
{

public:
    LeafVolatilization(QString name, QObject *parent);
private:
    // Input
    double VP, Ea, Tref, Tair, load;
    // Output
    double evaporationRate, Tcorrection;
    // Methods
    double computeInstantaneous();
};

} //namespace


#endif
