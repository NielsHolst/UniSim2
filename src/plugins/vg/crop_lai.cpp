/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
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
    help("models crop LAI");
    Input(laiStartPerPlant).equals(0.3).help("LAI when the crop is established [m2 leaf area per m2 planted area]");
    Input(fractionPlantArea).equals(1.).help("Fraction of ground area covered by the crop [0;1]");
    Output(value).help("Crop LAI [m2 leaf area per m2 planted area]");
}

void CropLai::reset() {
    value = laiStartPerPlant;
}

} //namespace

