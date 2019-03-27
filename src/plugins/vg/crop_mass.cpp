/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "crop_mass.h"

using namespace base;

namespace vg {

CropMass::CropMass(QString name, QObject *parent)
	: Box(name, parent)
{
    Class(CropMass);
    help("models crop growth and dry mass");
    Input(establishCrop).imports("../periods/*[flagUp]").transform(Any).unit("y|n");
    Input(removeCrop).imports("../periods/*[flagDown]").transform(Any).unit("y|n");
    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");
    Input(grossGrowthRate).imports("../growth[grossGrowthRate]").unit("g/m2/h");
    Input(initMass).equals(10).help("Initial total crop dry mass").unit("g/m2");
    Input(propRoot).equals(0.).help("Initial proportion of crop dry mass in root").unit("[0;1]");
    Input(propStem).equals(0.5).help("Initial proportion of crop dry mass in stem").unit("[0;1]");
    Input(propLeaf).equals(0.5).help("Initial proportion of crop dry mass in leaf").unit("[0;1]");
    Input(propFruit).equals(0.).help("Initial proportion of crop dry mass in fruit").unit("[0;1]");
    Input(costRoot).equals(0.39).help("Relative cost of conversion to root mass").unit("[0;1]");
    Input(costStem).equals(0.45).help("Relative cost of conversion to stem mass").unit("[0;1]");
    Input(costLeaf).equals(0.39).help("Relative cost of conversion to leaf mass").unit("[0;1]");
    Input(costFruit).equals(0.35).help("Relative cost of conversion to fruit mass").unit("[0;1]");

    Output(root).help("Root dry mass").unit("g/m2");
    Output(stem).help("Stem dry mass").unit("g/m2");
    Output(leaf).help("Leaf dry mass").unit("g/m2");
    Output(fruit).help("Fruit dry mass").unit("g/m2");
    Output(total).help("Total dry mass").unit("g/m2");

    Output(rootGrowthRate).help("Growth rate of root dry mass").unit("g/m2/h");
    Output(stemGrowthRate).help("Growth rate of stem dry mass").unit("g/m2/h");
    Output(leafGrowthRate).help("Growth rate of leaf dry mass").unit("g/m2/h");
    Output(fruitGrowthRate).help("Growth rate of fruit dry mass").unit("g/m2/h");
    Output(totalGrowthRate).help("Growth rate of total dry mass").unit("g/m2/h");
}

void CropMass::reset() {
    remove();
}

void CropMass::remove() {
    root = stem = leaf = fruit = total =
    rootGrowthRate = stemGrowthRate = leafGrowthRate = fruitGrowthRate = totalGrowthRate = 0.;
}

void CropMass::update() {
    setProportions();
    if (establishCrop)
        establish();
    else if (removeCrop)
        remove();
    allocate();
}

void CropMass::establish() {
    root = propRoot*initMass;
    stem = propStem*initMass;
    leaf = propLeaf*initMass;
    fruit = propFruit*initMass;
    total = root + stem + leaf + fruit;
}

void CropMass::allocate() {
    // Compute growth increments
    rootGrowthRate = propRoot*grossGrowthRate/(1+costRoot);
    stemGrowthRate = propStem*grossGrowthRate/(1+costStem);
    leafGrowthRate = propLeaf*grossGrowthRate/(1+costLeaf);
    fruitGrowthRate = propFruit*grossGrowthRate/(1+costFruit);
    totalGrowthRate = rootGrowthRate + stemGrowthRate + leafGrowthRate + fruitGrowthRate;
    // Add growth increments
    root += rootGrowthRate;
    stem += stemGrowthRate;
    leaf += leafGrowthRate;
    fruit += fruitGrowthRate;
    total += totalGrowthRate;
    // Convert growth increments to rates per hour
    rootGrowthRate *= 3600/timeStep;
    stemGrowthRate *= 3600/timeStep;
    leafGrowthRate *= 3600/timeStep;
    fruitGrowthRate *= 3600/timeStep;
    totalGrowthRate *= 3600/timeStep;
}

} //namespace

