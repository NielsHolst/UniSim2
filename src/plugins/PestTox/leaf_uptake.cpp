/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_uptake.h"
#include <cmath>
#include <base/exception.h>
#include <base/publish.h>
#include "general.h"

using namespace base;

namespace PestTox {

PUBLISH (LeafUptake)
	
LeafUptake::LeafUptake(QString name, QObject *parent)
    : LossRate(name, parent)
{
    Input(leafType).equals("NonWaxy").help("Either 'Waxy' or 'NonWaxy'");
    Input(Tair).imports("weather[Tavg]");
    Input(MV).imports("applications[molarVolume]");
}

double LeafUptake::computeInstantaneous() {
    double V = 0.945*MV + 2.772,
           B = R*2.303*(Tair + T0),
           log_klu;

    if (leafType == "NonWaxy")
        log_klu = -4.93 + 5.30e-3*V - 63.222*exp(2.874*MV/1000)/B;
    else if (leafType == "Waxy")
        log_klu = -5.31 + 5.84e-3*V - 78.875*exp(3.470*MV/1000)/B;
    else
        ThrowException("LeafType must be either 'Waxy' or 'NonWaxy").value(leafType).context(this);

    return pow(10., log_klu);       // per second
}

} //namespace

