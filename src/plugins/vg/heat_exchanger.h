/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HEAT_EXCHANGER_H
#define HEAT_EXCHANGER_H
#include <base/box.h>


namespace vg {

class HeatExchanger : public base::Box
{
public: 
    HeatExchanger(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double groundArea, outdoorsTemperature, indoorsTemperature, indoorsRh,
        efficiency, maxFlowRate, maxPowerUserHeatExchange, maxPowerUserParasitic,
        state;
    int number;
    // Outputs
    double
        condensationRate,
        powerUseCooling,
        powerUse,
        powerUserParasitic,
        powerUseheat;
};

} //namespace
#endif
