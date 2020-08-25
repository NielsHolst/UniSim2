/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "controller_ventilation_combined.h"

using namespace base;

namespace vg {
	
PUBLISH(ControllerVentilationCombined)

ControllerVentilationCombined::ControllerVentilationCombined(QString name, QObject *parent)
	: Box(name, parent)
{
    help("combines controllers for ventilation");
    Input(controllerVentilation).imports("controllers/ventilation[value]", CA);
    Input(controllerCrackVentilation).imports("controllers/crackVentilation[value]", CA);
    Output(value).help("Combined control");
}

void ControllerVentilationCombined::reset() {
    update();
}

void ControllerVentilationCombined::update() {
    value = (controllerVentilation < 0. && controllerCrackVentilation < 0.) ?
            // If both want to close, let the largest desire win
            std::min(controllerVentilation, controllerCrackVentilation) :
            // If either or both want to open, let the largest desire win
            std::max(controllerVentilation, controllerCrackVentilation);
}

} //namespace
