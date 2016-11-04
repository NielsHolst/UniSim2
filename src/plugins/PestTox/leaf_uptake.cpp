/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_uptake.h"
#include <cmath>
#include <base/publish.h>
#include "general.h"

using namespace base;

namespace PestTox {

PUBLISH (LeafUptake)
	
LeafUptake::LeafUptake(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(a).equals(0.);
    Input(b).equals(0.);
    Input(Tair).equals(25.);             //deg C
    Input(MV).equals(140.83);         //cm3/mol
    Input(leafType).equals(1);        //types of leaves, citrus = 1, pear = 2

    Output(Ed);
    Output(kl);               //rate constant of the uptake process, day-1
    Output(log_kl);           //log10(k)
    Output(V);
}

void LeafUptake::update() {

    V = (0.9445 * MV) + 2.7717;
    Ed = (leafType == 1) ? 78.875 * exp(3.470*MV/1000.) : 63.222 * exp(2.874*MV/1000.);
    if(leafType == 1)
        a = 0.8, b = -0.1167;
    else
        a = 3.231, b = 6.341e-2;
    log_kl  = a - b*V - (Ed*1000)/(R*2.3*(Tair + T0));
    kl = pow(10, log_kl)*3600.*24.;

}

} //namespace

