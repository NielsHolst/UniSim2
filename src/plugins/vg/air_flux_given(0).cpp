/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/product.h>
#include <base/publish.h>
#include "air_flux_given.h"

namespace vg {

PUBLISH(AirFluxGiven)

AirFluxGiven::AirFluxGiven(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes total given air flux");
//    Output(value).imports("./*[value]").transform(Sum);
}


} //namespace

