/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "actuator_ventilation.h"

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(ActuatorVentilation)

ActuatorVentilation::ActuatorVentilation(QString name, QObject *parent)
	: Box(name, parent)
{
    help("sets current vent opening");
    Input(value).help("Current opening of vents").unit("[0;1]");
    Input(minValue).imports("setpoints/crackVentilation[value]", CA).help("Minimum opening of vents").unit("[0;1]");
    Input(maxValue).equals(1.).help("Maximum opening of vents").unit("[0;1]");
}

void ActuatorVentilation::reset() {
    value = minValue;
}

void ActuatorVentilation::update() {
    if (TestNum::ltZero(minValue) || TestNum::gt(minValue, 1.))
        ThrowException("Minimum ventilation opening must be inside [0;1]").value(minValue).context(this);
    if (value < minValue)
        value = minValue;
    else if (value > maxValue)
        value = maxValue;
}

double ActuatorVentilation::getOpening() const {
    return value;
}

void ActuatorVentilation::setOpening(double opening) {
    value = std::max(opening, minValue);
}

} //namespace

