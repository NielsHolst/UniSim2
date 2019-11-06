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
//    Input(screenMaxAtHighRh).equals(0.9).help("Limit on screens drawn at high RH").unit("[0;1]");
//    Input(screenEnergyThreshold).equals(5.).help("Draw energy screen below this sunlight level").unit("W/m2");
//    Input(screenEnergyThresholdBand).equals(5.).help("Width of energy screen response").unit("W/m2");
//    Input(screenShadeThreshold).equals(500.).help("Draw shade screen above this sunlight level").unit("W/m2");
//    Input(screenShadeThresholdBand).equals(50.).help("Width of shade screen response").unit("W/m2");
//    Input(screenBlackoutFromTimeFloat).help("Draw blackout screen at this time (day fraction)").unit("[0;1]");
//    Input(screenBlackoutToTimeFloat).help("Withdraw blackout screen at this time (day fraction)").unit("[0;1]");
    Input(chalk).help("Effect of chalk").unit("[0;1]");
//    Output(screenBlackoutFromTime).help("Draw blackout screen at this time").unit("h:m:s");
//    Output(screenBlackoutToTime).help("Withdraw blackout screen at this time").unit("h:m:s");
}

//namespace {
//    QTime dayFractionToTime(double fraction) {
//         QTime time(0,0);
//         return time.addSecs(static_cast<int>(24*60*60*fraction));
//    }
//}

//void ControllersInput::reset() {
//    screenBlackoutFromTime = dayFractionToTime(screenBlackoutFromTimeFloat);
//    screenBlackoutToTime = dayFractionToTime(screenBlackoutToTimeFloat);
//}

} //namespace

