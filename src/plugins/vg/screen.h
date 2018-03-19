/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_SCREEN_H
#define VG_SCREEN_H

#include <base/box.h>

namespace vg {

class Screen : public base::Box
{
public:
    Screen(QString name, QObject *parent);
    void reset();
    void initialize();
    void update();
    enum Orientation{Cover, Horizontal};
private:
    friend class Screens;

    // Inputs
    QString orientation;
    double shelterArea,
           transmissivityLight,
           emissivityInner, emissivityOuter,
           specificHeatCapacity,
           energyLossReduction, U, haze,
           transmissivityAir, transmissivityAirExponent, state;
    // Outputs
    int layer;
    double
        transmissivityLightNet,
        absorptivityLwInnerNet, absorptivityLwOuterNet,
        unhazed, transmissivityAirNet, effectiveArea,
        resistance, heatCapacity;
    bool isHorizontal;
    // Data
    Orientation _orientation;
    // Methods
    void updateByState(double state);
};

} //namespace


#endif
