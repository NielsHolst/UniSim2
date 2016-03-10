/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "fruit_crop_lai.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(FruitCropLai)

/*! \class FruitCropLai
 * \brief LAI of fruit crops, such as cucumber and tomato
 *
 * Inputs
 * ------
 * - _maxLai_ is the max. LAI of the crop [m<SUP>2</SUP> leaf/m<SUP>2</SUP> planted area]
 * - _density_ is the number of plants per ground area [m<SUP>-2</SUP>]
 * - _tempSum_ is the temperature sum since the crop was established [d <SUP>o</SUP>C]
 * - _cropPresent_ tells whether the crop is present [true, false]
 *
 *
 * Output
 * ------
 * - _maxLaiReached_ tells whether the max. LAI has been reached [true, false]
 */

FruitCropLai::FruitCropLai(QString name, QObject *parent)
    : CropLai(name, parent)
{
    Input(maxLai).equals(3.5);
    Input(cropDensity).imports("..[density]");
    Input(tempSum).imports("../physTime[total]");
    Input(cropPresent).imports("../periods[flag]");
    Output(maxLaiReached);
}

void FruitCropLai::reset() {
    laiStart = laiStartPerPlant*cropDensity;
    value = 0.;
    maxLaiReached = (value>=maxLai);
}

void FruitCropLai::update() {
    value = cropPresent ? max(laiStart, laiStart+exp(tempSum/700)-1.) : 0.;
    maxLaiReached = (value>=maxLai);
    if (maxLaiReached)
        value = maxLai;
}


} //namespace

