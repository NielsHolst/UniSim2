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
    help("compounds signal from boxes: periods, on and off ");
}

void GrowthLightController::amend() {
    periodFlag = getFlag("periods");
    onFlag = getFlag("on");
    offFlag = getFlag("off");
}

const bool * GrowthLightController::getFlag(QString name) {
    QString path{"./%1[flag]"};
    Port *port = Path(path.arg(name), this).resolveOne<Port>(this);
    return port->valuePtr<bool>();
}

double GrowthLightController::computeSignal() {
    bool isOn = *periodFlag && *onFlag && !(*offFlag);
    return isOn ? 1 : 0;
}

} //namespace

