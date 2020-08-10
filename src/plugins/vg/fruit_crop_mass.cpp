/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/test_num.h>
#include "fruit_crop_mass.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(FruitCropMass)

FruitCropMass::FruitCropMass(QString name, QObject *parent)
    : CropMass(name, parent)
{
    help("models growth and dry mass for a fruit crop");
    Input(vegPropRoot).equals(0.) .help("Proportion of growth in vegetative stage").unit("[0;1]");
    Input(vegPropStem).equals(0.5).help("Proportion of growth in vegetative stage").unit("[0;1]");
    Input(vegPropLeaf).equals(0.5).help("Proportion of growth in vegetative stage").unit("[0;1]");
    Input(vegPropFruit).equals(0.).help("Proportion of growth in vegetative stage").unit("[0;1]");
    Input(reproPropRoot).equals(0.).help("Proportion of growth in reproductive stage").unit("[0;1]");
    Input(reproPropStem).equals(0.).help("Proportion of growth in reproductive stage").unit("[0;1]");
    Input(reproPropLeaf).equals(0.).help("Proportion of growth in reproductive stage").unit("[0;1]");
    Input(reproPropFruit).equals(1.).help("Proportion of growth in reproductive stage").unit("[0;1]");
    Input(tempSumReproductive).equals(975.).help("Temperature sum when the reproductive stage is reached").unit("DD");
    Input(tempMaxReproductive).equals(30.).help("Max. temperature for plant producing fruits").unit("oC");
    Input(fruitFactor).equals(1.).help("Factor multiplied on propFruit").unit("-");
    Input(indoorsTemperature).imports("indoors[temperature]",CA).unit("oC");
    Input(tempSum).imports("../physTime[total]",CA).unit("DD");
    Input(lai).imports("../lai[value]",CA).unit("m2/m2");
}

void FruitCropMass::setProportions() {
    bool isVegetative = (tempSum<tempSumReproductive || indoorsTemperature>tempMaxReproductive);
    if (isVegetative) {
        propRoot = vegPropRoot;
        propLeaf = vegPropLeaf;
        propStem = vegPropStem;
        propFruit = vegPropFruit;
    }
    else {
        propRoot = reproPropRoot;
        propLeaf = reproPropLeaf;
        propStem = reproPropStem;
        propFruit = reproPropFruit;
    }
    // Correct fruit and adjust others
    propFruit *= fruitFactor;
    double nonFruit0 = propRoot + propLeaf + propStem,
           nonFruit = 1. - propFruit;
    if (nonFruit0 > 0) {
        double c = nonFruit/nonFruit0;
        propRoot *= c;
        propStem *= c;
        propLeaf *= c;
    }
    double total = propRoot + propStem + propLeaf + propFruit;
    if (TestNum::ne(total, 1.)) {
        QString msg {"Proportions do not add up to 1: root(%1)+stem(%2)+leaf(%3)+fruit(%4)=%5"};
        ThrowException(msg.arg(propRoot).arg(propStem).arg(propLeaf).arg(propFruit).arg(total)).
                       value(total).context(this);
    }
}

} //namespace

