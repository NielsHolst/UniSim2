/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "air_flux_given.h"

using namespace base;

namespace vg {

PUBLISH(AirFluxGiven)

/*! \class AirFluxGiven
 * \brief  The air flux given, irrespective of temperature-regulated ventilation

 *
 * Inputs
 * ------
 * - _infiltration_ is the infiltration air exchange rate [h<SUP>-1</SUP>]
 * - _crackVentilation_ is the air flux through the humidity-controlled ventilation crack [h<SUP>-1</SUP>]
 *
 * Output
 * ------
 * - _value_ is the relative rate of air exchanged [h<SUP>-1</SUP>]
 */

AirFluxGiven::AirFluxGiven(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(AirFluxGiven);
    Input(infiltration).imports("./infiltration[value]");
    Input(crackVentilation).imports("./crackVentilation[value]");
    Input(gravitation).imports("./gravitation[value]");
    Input(transmissivity).imports("construction/shelters[airTransmissivity]");
    Output(value);
}

void AirFluxGiven::reset() {
    value = 0.;
}

void AirFluxGiven::update() {
    value = infiltration + transmissivity*crackVentilation + gravitation;
}


} //namespace

