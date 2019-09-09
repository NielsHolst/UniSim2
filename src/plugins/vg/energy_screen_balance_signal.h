/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_ENERGY_SCREEN_BALANCE_SIGNAL_H
#define VG_ENERGY_SCREEN_BALANCE_SIGNAL_H

#include "base_signal.h"

namespace vg {

class EnergyScreenBalanceSignal : public BaseSignal
{
public:
    EnergyScreenBalanceSignal(QString name, QObject *parent);
    double computeSignal();
private:
    // Inputs
    double indoorsLight, indoorsTemperature, outdoorsTemperature, Ucover;
};

} //namespace


#endif
