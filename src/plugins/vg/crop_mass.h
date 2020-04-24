/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CROP_MASS_H
#define CROP_MASS_H

#include <base/box.h>

namespace vg {

class CropMass : public base::Box
{
public:
    CropMass(QString name, QObject *parent);
    void reset();
    void update() final;
    virtual void setProportions() = 0;
protected:
    // Inputs
    bool establishCrop, removeCrop;
    double timeStep, initMass, growthRate,
        propRoot, propStem, propLeaf, propFruit,
        costRoot, costStem, costLeaf, costFruit;
    // Outputs
    double
        root, stem, leaf, fruit, total,
        rootGrowthRate, stemGrowthRate, leafGrowthRate, fruitGrowthRate, totalGrowthRate;
    // Methods
    void allocate();
private:
    void establish();
    void remove();
};
} //namespace


#endif
