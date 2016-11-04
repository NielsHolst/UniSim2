/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "soil_temperature.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (SoilTempetarure)
	
SoilTempetarure::SoilTempetarure(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(airTemperature).equals(110.);
    Output(value);
}

void SoilTempetarure::reset() {
    update();
}

void SoilTempetarure::update() {
    value = 1.05*airTemperature - 1.5;
}

} //namespace

