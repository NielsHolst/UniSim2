/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ENERGY_SCREEN_BALANCE_SIGNAL_H
#define ENERGY_SCREEN_BALANCE_SIGNAL_H

#include <base/base_signal.h>

namespace vg {

class EnergyScreenBalanceSignal : public base::BaseSignal
{
public:
    EnergyScreenBalanceSignal(QString name, QObject *parent);
    bool computeFlag();
    double computeSignal(bool flag);
private:
    // Inputs
    double indoorsLight, indoorsTemperature, outdoorsTemperature, Ucover;
};

} //namespace


#endif
