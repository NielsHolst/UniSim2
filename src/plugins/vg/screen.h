/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SCREEN_H
#define SCREEN_H
#include "radiation_layer.h"
#include <base/box.h>

namespace vg {

class Screen : public RadiationLayer
{
public:
    Screen(QString name, QObject *parent);
    void reset();
    void initialize();
    void update();
private:
    friend class Screens;

    // Inputs
    double haze, transmissivityAir, transmissivityAirExponent, state;
    // Outputs
    int layer;
    double
        unhazed, transmissivityAirNet, effectiveArea,
        heatCapacity;
    // Methods
    void updateByState(double state);
};

} //namespace


#endif
