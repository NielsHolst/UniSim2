/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SCREEN_H
#define SCREEN_H
#include "heat_transfer_layer_parameters.h"
#include <base/box.h>

namespace vg {

class Screen : public HeatTransferLayerParameters
{
public:
    Screen(QString name, QObject *parent);
    void reset();
    void update();
private:
    friend class Screens;

    // Inputs
    double haze, transmissivityAir, state;
    // Outputs
    double
        swReflectivityTopNet, swReflectivityBottomNet,
        swTransmissivityTopNet, swTransmissivityBottomNet,
        lwReflectivityTopNet, lwReflectivityBottomNet,
        lwTransmissivityTopNet, lwTransmissivityBottomNet,
        swAbsorptivityTopNet, swAbsorptivityBottomNet,
        lwAbsorptivityTopNet, lwAbsorptivityBottomNet,
        maxArea, depth,
        heatCapacity;
    // Methods
    void updateByState(double state);
    virtual double computeMaxArea() = 0;
    virtual double computeDepth() = 0;
};

} //namespace


#endif
