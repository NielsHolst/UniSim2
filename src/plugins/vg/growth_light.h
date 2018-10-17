/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_GROWTH_LIGHT_H
#define VG_GROWTH_LIGHT_H

#include "growth_light_base.h"

namespace vg {

class GrowthLight : public GrowthLightBase {
public:
    GrowthLight(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QString type;
    double timeStep, intensity, parPhotonCoef,
        minPeriodOn, age, lifeTime;
    bool on;
    // Outputs
    double
        currentPeriod, totalPeriod;
    // Data
    double _degradationRate;
    // Methods
    void noLight();
};
} //namespace


#endif
