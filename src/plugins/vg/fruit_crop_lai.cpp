/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "fruit_crop_lai.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(FruitCropLai)

FruitCropLai::FruitCropLai(QString name, QObject *parent)
    : CropLai(name, parent)
{
    help("models LAI of fruit crops");
    Input(maxLai).equals(3.5).help("Max. LAI in cropped area").unit("m2/m2");
    Input(cropDensity).imports("..[density]").unit("/m2");
    Input(tempSum).imports("../physTime[total]").unit("DD");
    Input(cropPresent).imports("../periods/*[flag]").transform(Any).unit("y|n");
    Output(maxLaiReached).help("Tells whether max. LAI has been reached").unit("y|n");
}

void FruitCropLai::reset() {
    value = laiStart = laiStartPerPlant*cropDensity;
    maxLaiReached = (value>=maxLai);
}

void FruitCropLai::update() {
    value = cropPresent ? max(laiStart, laiStart+exp(tempSum/700)-1.) : 0.;
    maxLaiReached = (value>=maxLai);
    if (maxLaiReached)
        value = maxLai;
}


} //namespace

