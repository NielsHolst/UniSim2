/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_FOOD_WEB_BOX_BASE
#define BASE_FOOD_WEB_BOX_BASE
#include "box.h"

namespace base {

class FoodWebBoxBase : public base::Box
{
public: 
    FoodWebBoxBase(QString name, QObject *parent);
protected:
    // Inputs
    double density, demand,
        loss, supply,
        supplyDemandRatio, lossRatio;
};

} //namespace
#endif
