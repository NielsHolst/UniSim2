/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "primary_distribution_soil.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (PrimaryDistributionSoil)
	
PrimaryDistributionSoil::PrimaryDistributionSoil(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(Doseappl).equals(0.);     //kg a.i/ha
    Input(Doserl).equals(0.);       //kg a.i/ha
    Input(Dosedrift).equals(0.);    //kg a.i/ha

    Output(Dosers);          //kg a.i/ha
}

void PrimaryDistributionSoil::update() {

    Dosers = Doseappl - (Doserl + Dosedrift);
}

} //namespace

