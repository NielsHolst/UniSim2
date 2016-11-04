/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "soil_porosity.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (SoilPorosity)
	
SoilPorosity::SoilPorosity(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(fa).equals(0.25);     //fraction of air in the soil
    Input(fw).equals(0.25);     //fraction of water in the soil

    Output(Sp);

}

void SoilPorosity::reset() {

    update();

}

void SoilPorosity::update() {

    Sp = fa + fw;

}

} //namespace

