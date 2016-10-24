/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_uptake.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (leafuptake)
	
leafuptake::leafuptake(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(a).equals(0.);
    Input(b).equals(0.);
    Input(R).equals(8.3145);          //gas constant
    Input(T).equals(25.);             //deg C
    Input(MV).equals(140.83);         //cm3/mol
    Input(leaftype).equals(1);        //types of leaves, citrus = 1, pear = 2

    Output(Ed);
    Output(klu);               //rate constant of the uptake process, day-1
    Output(log_klu);           //log10(k)
    Output(V);
}

void leafuptake::reset() {

    klu = 0.;
    log_klu = 0.;    
    V = 0.;
    Ed = 0.;
}

void leafuptake::update() {

    V = (0.9445 * MV) + 2.7717;
    Ed = (leaftype == 1) ? 78.875 * exp(3.470*MV/1000.) : 63.222 * exp(2.874*MV/1000.);
    if(leaftype == 1)
        a = 0.8, b = -0.1167;
    else
        a = 3.231, b = 6.341e-2;
    log_klu  = a - b*V - (Ed*1000)/(R*2.3*(T + 273.15));
    klu = pow(10, log_klu)*3600.*24.;

}

} //namespace

