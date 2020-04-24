/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RADIATION_LAYER_SKY_H
#define RADIATION_LAYER_SKY_H
#include "radiation_layer.h"

namespace vg {

class RadiationLayerSky : public RadiationLayer
{
public:
    RadiationLayerSky(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    double propPar, propUv;
};

} //namespace


#endif
