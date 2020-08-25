/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "controller_vent_max.h"

using namespace base;

namespace vg {

PUBLISH(ControllerVentMax)

ControllerVentMax::ControllerVentMax(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes maximum vent opening");
    Input(indoorsTemperature).imports("indoors[temperature]",CA);
    Input(outdoorsTemperature).imports("outdoors[temperature]",CA);
    Input(crackVentilation).imports("../min[value]");
    Output(value).help("Relative rate of air exchange [m3/m3/h]");
}

void ControllerVentMax::reset() {
    update();
}

void ControllerVentMax::update() {
    value = (indoorsTemperature > outdoorsTemperature) ? 1. : crackVentilation;
}

} //namespace

