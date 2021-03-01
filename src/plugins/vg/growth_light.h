/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GROWTH_LIGHT_H
#define GROWTH_LIGHT_H

#include "heat_transfer_layer_base.h"

namespace vg {

class GrowthLight : public HeatTransferLayerBase {
public:
    GrowthLight(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double intensity, parPhotonCoef, propLw,
        ageCorrectedEfficiency,
        minPeriodOn;
    bool on;
    // Outputs
    double
        periodOn, powerUsage;
    bool currentlyOn;
    // Methods
    void noLight();
};
} //namespace


#endif
