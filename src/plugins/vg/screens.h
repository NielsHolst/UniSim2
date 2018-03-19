/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_SCREENS_H
#define VG_SCREENS_H

#include "surface_radiation_outputs.h"

namespace vg {

class Screens : public vg::SurfaceRadiationOutputs
{
public:
    Screens(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Outputs
    double U, heatCapacity, haze, airTransmissivity, effectiveArea, maxState;
    bool areHorizontal;

    // Data
    struct ScreenInfo {
        const bool *isHorizontal;
        const double *transmissivityLightNet,
            *absorptivityLwInnerNet, *absorptivityLwOuterNet,
            *state,
            *unhazed, *airTransmissionNet,
            *resistance, *heatCapacity, *effectiveArea;
    };
    QVector<ScreenInfo> screenInfos;

    // Methods
    QVector<ScreenInfo> collectScreenInfos(QVector<Box *> screenModels);
    void updateRadiation();
};

} //namespace


#endif
