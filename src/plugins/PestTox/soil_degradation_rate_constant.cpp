/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "soil_degradation_rate_constant.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (soildegradationrateconstant)
	
soildegradationrateconstant::soildegradationrateconstant(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(DT50).equals(1.);     //half-life in the soil (days)

    Output(k);           //soil degradation rate constant (day-1)

}

void soildegradationrateconstant::reset() {

    k = 0.;

}

void soildegradationrateconstant::update() {

    k = 0.693/DT50;

}

} //namespace

