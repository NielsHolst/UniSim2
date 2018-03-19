/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_FRUIT_CROP_MASS_H
#define VG_FRUIT_CROP_MASS_H

#include "crop_mass.h"

namespace vg {

class FruitCropMass : public CropMass
{
public:
    FruitCropMass(QString name, QObject *parent);
    void setProportions();

private:
    double indoorsTemperature, tempSum, maxLai, fruitFactor,
        tempSumReproductive, tempMaxReproductive, lai,
        vegPropRoot, vegPropStem, vegPropLeaf, vegPropFruit,
        reproPropRoot, reproPropStem, reproPropLeaf, reproPropFruit;
    bool maxLaiReached;
    void reallocateToFruits();
};
} //namespace


#endif
