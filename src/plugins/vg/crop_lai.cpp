/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "crop_lai.h"

using namespace base;

namespace vg {

PUBLISH(CropLai)

CropLai::CropLai(QString name, QObject *parent)
	: Box(name, parent)
{
    help("models LAI in cropped area");
    Input(laiStartPerPlant).equals(0.3).help("Initial crop LAI in cropped area").unit("m2/m2");
    Input(fractionPlantArea).equals(1.).help("Fraction of cropped area").unit("[0;1]");
    Output(value).help("Current LAI in cropped area]").unit("m2/m2");
}

void CropLai::reset() {
    value = laiStartPerPlant;
}

} //namespace

