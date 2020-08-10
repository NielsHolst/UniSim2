/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "energy_screen_balance_signal.h"
#include <base/publish.h>

using namespace base;

namespace vg {
	
PUBLISH(EnergyScreenBalanceSignal)

EnergyScreenBalanceSignal::EnergyScreenBalanceSignal(QString name, QObject *parent)
    : BaseSignal(name, parent)
{
    help("signals whether an energy screen should be drawn");
    Input(Ucover).imports("construction/shelter[U]").unit("W/m2/K");
    Input(indoorsLight).imports("indoors/light[total]").unit("W/m2");
    Input(indoorsTemperature).imports("indoors[temperature]").unit("oC");
    Input(outdoorsTemperature).imports("outdoors[temperature]").unit("oC");
}

double EnergyScreenBalanceSignal::computeSignal(bool flag)  {
    return flag ? 1. : 0.;
}

bool EnergyScreenBalanceSignal::computeFlag()  {
    return (indoorsLight < Ucover*(indoorsTemperature - outdoorsTemperature));
}

} //namespace
