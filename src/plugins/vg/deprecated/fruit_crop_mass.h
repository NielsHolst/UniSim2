/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FRUIT_CROP_MASS_H
#define FRUIT_CROP_MASS_H

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
    void reallocateToFruits();
};
} //namespace


#endif
