/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_GROWTH_LIGHT_CONTROLLER_H
#define VG_GROWTH_LIGHT_CONTROLLER_H

#include "base_signal.h"

namespace vg {

class GrowthLightController : public BaseSignal
{
public:
    GrowthLightController(QString name, QObject *parent);
    double computeSignal();
private:
    // Inputs
    bool isActive;
    double lightThresholdLow, lightThresholdHigh, lightOutdoors;
};
} //namespace


#endif
