/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "surface_radiation.h"
#include "surface_radiation_outputs.h"

using namespace base;

namespace vg {

/*! \class SurfaceRadiationOutputs
 * \brief Common surface radiation outputs
 *
 * Outputs
 * ------
 * - _lightTransmissivity_ is the transmissivity for diffuse light [0;1]
 * - _directLightTransmissivity_ is the transmissivity for direct light [0;1]
 * - _lwTransmissivity_ is the transmissivity for long-waved radiation [0;1]
 * - _incomingLightAbsorptivity_ is the absorptivity for diffuse light coming from the outdoors direction [0;1]
 * - _incomingLightReflectivity_ is the reflectivity for diffuse light coming from the outdoors direction [0;1]
 * - _incomingDirectLightAbsorptivity_ is the absorptivity for direct light coming from the outdoors direction [0;1]
 * - _incomingDirectLightReflectivity_ is the reflectivity for direct light coming from the outdoors direction [0;1]
 * - _incomingLwAbsorptivity_ is the absorptivity for long-waved radiation coming from the outdoors direction [0;1]
 * - _incomingLwReflectivity_ is the reflectivity for long-waved radiation coming from the outdoors direction [0;1]
 * - _outgoingLightAbsorptivity_ is the absorptivity for diffuse light going in the outdoors direction [0;1]
 * - _outgoingLightReflectivity_ is the reflectivity for diffuse light going in the outdoors direction [0;1]
 * - _outgoingDirectLightAbsorptivity_ is the absorptivity for direct light going in the outdoors direction [0;1]
 * - _outgoingDirectLightReflectivity_ is the reflectivity for direct light going in the outdoors direction [0;1]
 * - _outgoingLwAbsorptivity_ is the absorptivity for long-waved radiation going in the outdoors direction [0;1]
 * - _outgoingLwReflectivity_ is the reflectivity for long-waved radiation going in the outdoors direction [0;1]
 */

SurfaceRadiationOutputs::SurfaceRadiationOutputs(QString name, QObject *parent)
	: Box(name, parent)
{
    Class(SurfaceRadiationOutputs);
    Output(lightTransmissivity);
    Output(directLightTransmissivity);
    Output(lwTransmissivity);
    Output(incomingLightAbsorptivity);
    Output(incomingLightReflectivity);
    Output(incomingDirectLightAbsorptivity);
    Output(incomingDirectLightReflectivity);
    Output(incomingLwAbsorptivity);
    Output(incomingLwReflectivity);
    Output(outgoingLightAbsorptivity);
    Output(outgoingLightReflectivity);
    Output(outgoingDirectLightAbsorptivity);
    Output(outgoingDirectLightReflectivity);
    Output(outgoingLwAbsorptivity);
    Output(outgoingLwReflectivity);
}

void SurfaceRadiationOutputs::resetRadiationOutputs() {
    set(SurfaceRadiation());
}

void SurfaceRadiationOutputs::set(const SurfaceRadiation &rad_) {
    rad = rad_;
    lightTransmissivity = rad.light.tra;
    directLightTransmissivity = rad.directLight.tra;
    lwTransmissivity = rad.lw.tra;
    incomingLightAbsorptivity = rad.light.outer.abs;
    incomingLightReflectivity = rad.light.outer.ref;
    incomingDirectLightAbsorptivity = rad.directLight.outer.abs;
    incomingDirectLightReflectivity = rad.directLight.outer.ref;
    incomingLwAbsorptivity = rad.lw.outer.abs;
    incomingLwReflectivity = rad.lw.outer.ref;
    incomingLightAbsorptivity = rad.light.outer.abs;
    incomingLightReflectivity = rad.light.outer.ref;
    incomingDirectLightAbsorptivity = rad.directLight.outer.abs;
    incomingDirectLightReflectivity = rad.directLight.outer.ref;
    outgoingLwAbsorptivity = rad.lw.inner.abs;
    outgoingLwReflectivity = rad.lw.inner.ref;
}

const SurfaceRadiation* SurfaceRadiationOutputs::surfaceRadiation() const {
    return &rad;
}

} //namespace

