/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <math.h>
#include <base/exception.h>
#include <base/publish.h>
#include "general.h"
#include "vent.h"

using namespace base;

namespace vg {
	
PUBLISH(Vent)

Vent::Vent(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes vent effective area");
    Input(length).imports("construction/geometry[length]").unit("m");
    Input(width).equals(1).help("Width of one window (m)").unit("m");
    Input(number).equals(1).help("Number of windows").unit("-");
    Input(state).imports("actuators/vents[value]").help("Relative vent opening").unit("[0;1]");
    Input(ventTransmissivity).equals(1.).help("Air transmissivity through vent").unit("[0;1]");
    Input(screensTransmissivity).imports("../screens[airTransmissivity]")
            .help("Air transmissivity through all screens").unit("[0;1]");
    Output(transmissivity).help("Net air transmissivity").unit("[0;1]");
}

void Vent::reset() {
    update();
}

void Vent::update() {
    transmissivity = ventTransmissivity*screensTransmissivity;
}

} //namespace


