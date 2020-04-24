/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RADIATION_LAYER_CROP_H
#define RADIATION_LAYER_CROP_H
#include "radiation_layer.h"

namespace vg {

class RadiationLayerCrop : public RadiationLayer
{
friend class RadiationLayers;
public:
    RadiationLayerCrop(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        lai,
        swK, swScatteringCoef,
        lwK, lwScatteringCoef;
    struct Distribution {
        double a, r, t;
    };
    Distribution distribute(double k, double scatteringCoef) const;
};

} //namespace


#endif
