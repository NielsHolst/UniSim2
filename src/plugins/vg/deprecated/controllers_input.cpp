/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "controllers_input.h"

using namespace base;

namespace vg {

ControllersInput::ControllersInput(QString name, QObject *parent)
    : Box(name, parent)
{
    help("contains input ports for actuators");
    Input(ventilationThresholdBand).equals(1.).help("Width of ventilation temperature response").unit("oC");
    Input(crackVentilation).equals(0.05).help("Ventilation crack; reduced at low temperarure").unit("[0;1]");
    Input(crackVentilationTemperatureMin).equals(-5.).help("Threshold temperature at which crack is reduced").unit("oC");
    Input(crackVentilationTemperatureMinBand).equals(1).help("Width of crack temperature response").unit("oC");
    Input(chalk).help("Effect of chalk").unit("[0;1]");
}

} //namespace

