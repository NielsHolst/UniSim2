/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "crop_yield.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(CropYield)

CropYield::CropYield(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(produceMass).imports("../mass[fruit]").unit("g/m2");
    Input(fractionPlantArea).imports("crop/lai[fractionPlantArea]").unit("[0;1]");
    Input(fractionDryWeight).equals(0.03).help("Fraction of produce dry weight").unit("[0;1]");
    Output(freshWeight).help("Accumulated fresh weight of harvestable produce [kg per m2 greenhouse area]").unit("kg/m2");
    Output(dryWeight).help("Accumulated dry weight of harvestable produce [kg per m2 greenhouse area]").unit("kg/m2");
}

void CropYield::reset() {
    freshWeight = dryWeight = 0.;
}

void CropYield::update() {
    dryWeight = produceMass*fractionPlantArea/1000.;
    freshWeight = dryWeight/fractionDryWeight;
}

} //namespace

