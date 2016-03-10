/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_MASS_H
#define VG_CROP_MASS_H

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
    double timeStep, initMass, grossGrowthRate,
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
