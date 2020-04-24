/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RADIATION_LAYER_SHELTER_H
#define RADIATION_LAYER_SHELTER_H
#include <base/port.h>
#include "radiation_layer.h"

namespace vg {

class RadiationLayerShelter : public RadiationLayer
{
public:
    RadiationLayerShelter(QString name, QObject *parent, QString path);
    void initialize() final;
    void reset() final;
private:
    // Inputs
    QVector<double>
        swReflectivityTopShelter,   swReflectivityBottomShelter,
        swTransmissivityTopShelter, swTransmissivityBottomShelter,
        swAbsorptivityTopShelter,   swAbsorptivityBottomShelter,
        lwReflectivityTopShelter,   lwReflectivityBottomShelter,
        lwTransmissivityTopShelter, lwTransmissivityBottomShelter,
        lwAbsorptivityTopShelter,   lwAbsorptivityBottomShelter,
        UtopShelter, UbottomShelter,
        areaShelter;
    // Output
    int numFaces;
protected:
    // Methods
    void updateRadiativeProperties();
    void updateConvectiveProperties();
};

} //namespace


#endif
