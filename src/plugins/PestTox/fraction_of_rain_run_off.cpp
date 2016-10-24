/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "fraction_of_rain_run_off.h"
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (fractionofrainrunoff)
	
fractionofrainrunoff::fractionofrainrunoff(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(Q).equals(5.);
    Input(P).equals(1.);      //average  daily rainfall per rainfall event in a given month (mm)


    Output(frr);

}

void fractionofrainrunoff::reset() {

    frr = 0.;

}

void fractionofrainrunoff::update() {

    frr = (P > 0) ? Q/P : 0;

}

} //namespace

