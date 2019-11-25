/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "setpoints_input.h"

using namespace base;

namespace vg {

SetpointsInput::SetpointsInput(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(SetpointsInput);
    help("contains inputs and derived outputs for setpoints");
    Input(heatingTemperatureAtLowRh).equals(21.).help("Heat below this temperature at low RH").unit("oC");
    Input(heatingTemperatureMargin).equals(2.).help("Add this to heating temperature at high RH").unit("oC");
    Input(ventilationTemperatureMargin).equals(3.).help("Ventilate above this additional margin on heatingTemperature").unit("oC");
    Input(ventilationTemperatureRhMargin).equals(2.).help("Decrease ventilationTemperatureMargin by this at high RH").unit("oC");
    Input(rhMax).equals(80).help("Regulate RH below this level").unit("[0;100]");
    Input(rhMaxBand).equals(10).help("Proportional band for max. RH").unit("[0;100]");
    Input(screenCrackAtHighRh).equals(10.).help("Crack not covered by screen at high RH").unit("%");
    Input(screenCrackAtHighTemperature).equals(10.).help("Crack not covered by screen at high temperature").unit("5");
    Input(co2Capacity).equals(10.).help("Max. capacity for CO2 injection").unit("g/m2/h");
    Input(co2Setpoint).equals(900.).help("Desired CO2 concentration").unit("ppm");
    Input(co2VentilationThreshold).equals(0.1).help("Threshold for decreasing CO2 injection").unit("[0;1]");
    Input(co2VentilationBand).equals(0.2).help("Proportional band for decreasing CO2 injection").unit("[0;1]");
    Input(dawnThreshold).equals(0.2).help("Day begins above this sunlight intensity").unit("W/m2");
    Input(duskThreshold).equals(0.3).help("Night begins below this sunlight intensity").unit("W/m2");
    Output(ventilationTemperatureAtLowRh).help("Ventilate above this temperature at low RH").unit("oC");
    Output(ventilationTemperatureAtHighRh).help("Ventilate above this temperature at high RH").unit("oC");
    Output(heatingTemperatureAtHighRh).equals(21.).help("Heat below this temperature at high RH").unit("oC");
    Output(maxScreenAtHighRh).help("Max. screen state at high RH").unit("[0;1]");
    Output(maxScreenAtHighTemperature).help("Max. screen state at high temperature").unit("[0;1]");
}

void SetpointsInput::reset() {
    update();
}

void SetpointsInput::update() {
    // At low RH
    ventilationTemperatureAtLowRh = heatingTemperatureAtLowRh + ventilationTemperatureMargin;
    // At high RH
    heatingTemperatureAtHighRh    = heatingTemperatureAtLowRh + heatingTemperatureMargin;
    ventilationTemperatureAtHighRh = ventilationTemperatureAtLowRh + heatingTemperatureMargin
                                     - ventilationTemperatureRhMargin;
    maxScreenAtHighRh = 1. - screenCrackAtHighRh/100.;
    // At high temperature
    maxScreenAtHighTemperature = 1. - screenCrackAtHighTemperature/100.;
}

} //namespace

