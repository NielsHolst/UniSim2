/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/path.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "growth_light_controller.h"

using namespace base;

namespace vg {
	
PUBLISH(GrowthLightController)

/*! \class GrowthLightController
 * \brief Controls growth lights on/off
 *
 * The control is based on the signals set by three child models named "period", "on" and "off".
 *
 * Outputs
 * ------
 * - _signal_ is the on/off signal to lamps [true,false]
 */

GrowthLightController::GrowthLightController(QString name, QObject *parent)
	: Box(name, parent)
{
    Output(signal);
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

void GrowthLightController::reset() {
    signal = false;
}

void GrowthLightController::update() {
    signal = *periodFlag && *onFlag && !(*offFlag);
}

} //namespace

