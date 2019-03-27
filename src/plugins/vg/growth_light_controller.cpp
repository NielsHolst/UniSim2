/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/path.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "growth_light_controller.h"

using namespace base;

namespace vg {
	
PUBLISH(GrowthLightController)

GrowthLightController::GrowthLightController(QString name, QObject *parent)
    : BaseSignal(name, parent)
{
    help("compounds on and off signals");
    Input(isActive).imports("allSetpoints/growthLightActive[value]").help("Could light be on?").unit("y|n");
    Input(lightThresholdLow).imports("allSetpoints/growthLightThresholdLow[value]").help("Not used").unit("W/m2");
    Input(lightThresholdHigh).imports("allSetpoints/growthLightThresholdHigh[value]").unit("W/m2");
    Input(lightOutdoors).imports("outdoors[radiation]").unit("W/m2");
}

double GrowthLightController::computeSignal() {
    return isActive && (lightOutdoors < lightThresholdHigh);
}

} //namespace

