/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
    : BaseSignal(name, parent), _isOn(false)
{
    help("compounds on and off signals");
    Input(isActive).imports("allSetpoints/growthLightActive[value]").help("Could light be on?").unit("y|n");
    Input(lightThresholdLow).imports("allSetpoints/growthLightThresholdLow[value]")
            .help("Light is off below this threshold for outdoors light").unit("W/m2");
    Input(lightThresholdHigh).imports("allSetpoints/growthLightThresholdHigh[value]")
            .help("Light is off above this threshold for outdoors light").unit("W/m2");
    Input(lightOutdoors).imports("outdoors[radiation]").unit("W/m2");
}

double GrowthLightController::computeSignal() {
    if (isActive) {
        if (_isOn) {
            bool switchOff = (lightOutdoors > lightThresholdHigh);
            _isOn = !switchOff;
        }
        else {
            bool switchOn = (lightOutdoors < lightThresholdLow);
            _isOn = switchOn;
        }
    }
    else {
        _isOn = false;
    }
    return _isOn;
}

} //namespace

