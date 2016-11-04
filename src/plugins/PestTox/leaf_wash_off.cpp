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

PUBLISH (LeafWashOff)
	
LeafWashOff::LeafWashOff(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(P).equals(25.);      //rainfall (mm)
    Input(woc).equals(0.1);      //washoff coefficient
    Input(inflow).equals(0.); //kg a.i/ha
    Input(kov).equals(0.); // per day (day-1)

    Output(outflow); //kg a.i/ha
    Output(concentration); //kg a.i/ha
    Output(value); //kg a.i/ha
    //sum of the 3 leaf removal processes, or
    //dose of the pesticides reaching the leaves that are removed from the leaves before the next rain event
}

void LeafWashOff::update() {

    outflow = (concentration*kov) + (concentration*woc*P*0.1);
    concentration += (concentration < 0) ? 0: inflow - outflow;
    value = concentration*woc*P*0.1;

}

} //namespace

