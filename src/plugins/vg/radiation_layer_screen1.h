/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RADIATION_LAYER_SCREEN1_H
#define RADIATION_LAYER_SCREEN1_H
#include "radiation_layer_shelter.h"

namespace vg {

class RadiationLayerScreen1 : public RadiationLayerShelter
{
public:
    RadiationLayerScreen1(QString name, QObject *parent);
    void update();
};

} //namespace


#endif
