/* Copyright 2005-2018 byoC
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
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
    Input(heatingTemperatureAtLowRh).equals(21.).help("Heat below this temperature at low RH[oC]");
    Input(heatingTemperatureMargin).equals(2.).help("Add this to heating temperature at high RH []");
    Input(ventilationTemperatureMargin).equals(3.).help("Ventilate above this additional margin on heatingTemperature [oC]");
    Input(ventilationTemperatureRhMargin).equals(2.).help("Decrease ventilationTemperatureMargin by this at high RH [oC]");
    Input(rhMax).equals(80).help("Regulate RH below this level [0..100]");
    Input(rhMaxBand).equals(10).help("Proportional band for max. RH [0..100]");
    Input(co2Min).equals(400.).help("Minimum CO2 [ppm]");
    Input(co2Max).equals(900.).help("Maximum CO2 [ppm]");
    Input(dawnThreshold).equals(0.2).help("Day begins above this sunlight intensity [W/m2]");
    Input(duskThreshold).equals(0.3).help("Night begins below this sunlight intensity [W/m2]");
    Output(ventilationTemperatureAtLowRh).help("Ventilate above this temperature at low RH [oC]");
    Output(ventilationTemperatureAtHighRh).help("Ventilate above this temperature at high RH [oC]");
    Output(heatingTemperatureAtHighRh).equals(21.).help("Heat below this temperature at high RH[oC]");
}

void SetpointsInput::reset() {
    update();
}

void SetpointsInput::update() {
    ventilationTemperatureAtLowRh = heatingTemperatureAtLowRh + ventilationTemperatureMargin;
    ventilationTemperatureAtHighRh = ventilationTemperatureAtLowRh - ventilationTemperatureRhMargin;
    heatingTemperatureAtHighRh = heatingTemperatureAtLowRh + heatingTemperatureMargin;
}

} //namespace

