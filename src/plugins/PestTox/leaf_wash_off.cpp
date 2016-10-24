/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_wash_off.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (leafwashoff)
	
leafwashoff::leafwashoff(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(P).equals(25.);      //rainfall (mm)
    Input(woc).equals(0.1);      //washoff coefficient
    Input(inflow).equals(0.); //kg a.i/ha
    Input(kov).equals(0.); // per day (day-1)

    Output(outflow); //kg a.i/ha
    Output(concentration); //kg a.i/ha
    Output(washoff); //kg a.i/ha
    //sum of the 3 leaf removal processes, or
    //dose of the pesticides reaching the leaves that are removed from the leaves before the next rain event
}

void leafwashoff::reset() {

    washoff = 0.;
    outflow = 0.;
    concentration = 0.;
}

void leafwashoff::update() {

    outflow = (concentration*kov) + (concentration*woc*P*0.1);
    concentration += (concentration < 0) ? 0: inflow - outflow;
    washoff = concentration*woc*P*0.1;    

}

} //namespace

