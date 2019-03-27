/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "surface_radiation.h"
#include "surface_radiation_outputs.h"

using namespace base;

namespace vg {

SurfaceRadiationOutputs::SurfaceRadiationOutputs(QString name, QObject *parent)
	: Box(name, parent)
{
    Class(SurfaceRadiationOutputs);
    help("base class for surface radiation models");
    Output(lightTransmissivity).help("Transmissivity for diffuse light").unit("[0;1]");
    Output(directLightTransmissivity).help("Transmissivity for direct light").unit("[0;1]");
    Output(lwTransmissivity).help("Transmissivity for long-wave radiation").unit("[0;1]");
    Output(incomingLightAbsorptivity).help("Absorptivity for incoming diffuse light").unit("[0;1]");
    Output(incomingLightReflectivity).help("Reflectivity for incoming diffuse light").unit("[0;1]");
    Output(incomingDirectLightAbsorptivity).help("Absorptivity for incoming direct light").unit("[0;1]");
    Output(incomingDirectLightReflectivity).help("Reflectivity for incoming direct light").unit("[0;1]");
    Output(incomingLwAbsorptivity).help("Absorptivity for incoming long-wave radiation").unit("[0;1]");
    Output(incomingLwReflectivity).help("Reflectivity for incoming long-wave radiation").unit("[0;1]");
    Output(outgoingLightAbsorptivity).help("Absorptivity for outgoing diffuse light").unit("[0;1]");
    Output(outgoingLightReflectivity).help("Reflectivity for outgoing diffuse light").unit("[0;1]");
    Output(outgoingDirectLightAbsorptivity).help("Absorptivity for outgoing direct light").unit("[0;1]");
    Output(outgoingDirectLightReflectivity).help("Reflectivity for outgoing direct light").unit("[0;1]");
    Output(outgoingLwAbsorptivity).help("Absorptivity for outgoing long-wave radiation").unit("[0;1]");
    Output(outgoingLwReflectivity).help("Reflectivity for outgoing long-wave radiation").unit("[0;1]");
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

