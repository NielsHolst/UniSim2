/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef COFFEE_H
#define COFFEE_H
#include <base/box.h>

namespace coffee {

class Coffee : public base::Box
{
public: 
    Coffee(QString name, QObject *parent);
    void update();
private:
    // Inputs
    bool isHarvestTime;
    double
        gLai, dLai,
//        gLeafN, dLeafN,
        gLeafC, dLeafC,
        gWoodyC,
        gRootC, dRootC,
        gBerryC;
    // Outputs
    double
        lai,
        leafN,
        leafC, woodyC, rootC, berryC,
        harvestedBerryC;
};

} //namespace
#endif
