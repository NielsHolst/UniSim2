/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "growth_light_base.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(GrowthLightBase)

/*! \class GrowthLightBase
 * \brief Base class for growth light
 *
 * Output
 * ------
 * - _heatEmission_ is the convective heat emission [W/m<SUP>2</SUP>]
 * - _longWaveEmission_ is the long wave emission (thermal radiation, Bn) [W/m<SUP>2</SUP>]
 * - _shortWaveEmission_ is the short wave emission (visible light, Rn) [W/m<SUP>2</SUP>]
 * - _totalEmission_ is the total of _heatEmission_+_longWaveEmission_+_shortWaveEmission_  [W/m<SUP>2</SUP>]
 * - _parEmission_ is the PAR emision [W/m<SUP>2</SUP>]
 * - _energyFlux_ is the energy used for growth lights [W/m<SUP>2</SUP>]
 * - _currentlyOn_ tells whether light is on [true/false]
 */

GrowthLightBase::GrowthLightBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Output(heatEmission);
    Output(longWaveEmission);
    Output(shortWaveEmission);
    Output(totalEmission);
    Output(parEmission);
    Output(energyFlux);
    Output(bool, currentlyOn);
}

void GrowthLightBase::noLight() {
    heatEmission =
    longWaveEmission =
    shortWaveEmission =
    totalEmission =
    parEmission =
    energyFlux = 0.;
    currentlyOn = false;
}

} //namespace

