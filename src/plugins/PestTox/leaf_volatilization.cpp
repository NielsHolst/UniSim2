/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_volatilization.h"
#include <cmath>
#include <base/publish.h>
#include "general.h"

using namespace base;

namespace PestTox {
	
PUBLISH(LeafVolatilization)

LeafVolatilization::LeafVolatilization(QString name, QObject *parent)
    : LossRate(name, parent)
{
    Input(VP).imports("applications[vapourPressure]");
    Input(Ea).imports("applications[activationEnergyVolatilization]");
    Input(Tref).imports("applications[Tref]");
    Input(Tair).imports("weather[Tavg]");
    Input(load).imports("onCrop[load]");
    Output(Tcorrection).help("Temperature correction factor (scalar)");
    Output(evaporationRate).help("Evaporation rate from leaves (g/ha/h)");
}

double LeafVolatilization::computeInstantaneous() {
    double evaporationRateTref = exp(12.2 + 0.933 * log(VP)) * 1e-6 * 1e4; // g/ha/h = myg/m2/h * g/myg * m2/ha
    Tcorrection = Ea*1000./R*((1./(Tref + T0)) - 1./(Tair + T0)); // 1 = kJ/mol * J/KJ / (J/mol/K) / K
    evaporationRate = evaporationRateTref * exp(Tcorrection) / 3600; // g/ha/s = g/ha/h / (s/h)
    return (load > 0) ? evaporationRate/load : 0.; // s-1 = g/ha/s / (g/ha)
  }

} //namespace

