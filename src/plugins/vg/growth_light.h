/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
