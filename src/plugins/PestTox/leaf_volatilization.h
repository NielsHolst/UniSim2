/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
