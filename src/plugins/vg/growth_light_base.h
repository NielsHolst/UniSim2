/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_BASE_GROWTH_LIGHT_H
#define VG_BASE_GROWTH_LIGHT_H

#include <base/box.h>

namespace vg {

class GrowthLightBase : public base::Box
{
public:
    GrowthLightBase(QString name, QObject *parent);
protected:
    // Inputs
    double powerUsage;
    // Outputs
    double
        totalIntensity,
        shortWaveProp, shortWaveIntensity,
        longWaveProp, longWaveIntensity,
        heatProp, heatIntensity,
        parIntensity;
    bool currentlyOn;
};
} //namespace


#endif
