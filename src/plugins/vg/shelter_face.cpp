/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/box_builder.h>
#include <base/path.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "cover.h"
#include "screens.h"
#include "shelter_face.h"

using namespace base;
using phys_math::div0;

namespace vg {

PUBLISH(ShelterFace)

ShelterFace::ShelterFace(QString name, QObject *parent)
    : ShelterBase(name, parent)
{
    help("models a shelter face");
    Input(roofArea).imports("geometry[roofArea]").unit("m2");
    Input(sideWallsArea).imports("geometry[sideWallsArea]").unit("m2");
    Input(endWallsArea).imports("geometry[endWallsArea]").unit("m2");
    Input(gablesArea).imports("geometry[gablesArea]").unit("m2");
    Input(groundArea).imports("geometry[groundArea]").unit("m2");

    Input(outdoorsDirectRadiation).imports("outdoors[directRadiation]").unit("W/m2");
    Input(outdoorsDiffuseRadiation).imports("outdoors[diffuseRadiation]").unit("W/m2");

    Output(area).help("Surface area").unit("m2");
    Output(relativeArea).help("Proportion of this surface area out of the total surface area").unit("[0;1]");
    Output(areaPerGround).help("Surface area relative to the ground area").unit("m2/m2");
    Output(screensMaxState).help("Max. state of screens on this surface").unit("[0;1]");
}

void ShelterFace::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./cover"))
        builder.box("Cover").name("cover").endbox();
    if (objectName().contains("roof") && !findMaybeOne<Box>("./vent"))
        builder.box("Vent").name("vent").endbox();
}

void ShelterFace::initialize() {
    face.fetch(this);

    Cover *coverM = findOne<Cover>("./cover");
    pCoverU = coverM->port("U")->valuePtr<double>();
    pCoverHaze = coverM->port("haze")->valuePtr<double>();
    cover.fetch(coverM);
    pCoverSurfaceRadiation = coverM->surfaceRadiation();

    Screens *screensM = findMaybeOne<Screens>("./screens");
    _hasScreens = screensM;
    if (_hasScreens) {
        pScreensU = screensM->port("U")->valuePtr<double>();
        pScreensHaze = screensM->port("haze")->valuePtr<double>();
        screens.fetch(screensM);
        pScreensSurfaceRadiation = screensM->surfaceRadiation();
        pScreensAirTransmission = screensM->port("airTransmissivity")->valuePtr<double>();
    }
    _screens = findMany<Box>("./screens/*");
}

void ShelterFace::Light::fetch(base::Box *model) {
    diffuse.tra = model->port("lightTransmissivity")->valuePtr<double>();
    direct.tra =  model->port("directLightTransmissivity")->valuePtr<double>();
    diffuse.abs = model->port("incomingLightAbsorptivity")->valuePtr<double>();
    direct.abs =  model->port("incomingDirectLightAbsorptivity")->valuePtr<double>();
}

void ShelterFace::reset() {
    ShelterBase::reset();

    QString name = objectName();
    if (name.toLower().contains("roof"))
        area = roofArea/2;
    else if (name.toLower().contains("side"))
        area = sideWallsArea/2;
    else if (name.toLower().contains("end"))
        area = endWallsArea/2;
    else {
        QString msg{"Cover name is '%1' but it must contain 'roof', 'side' or 'end'"};
        ThrowException(msg.arg(name));
    }
    relativeArea = area/(roofArea + sideWallsArea + endWallsArea);
    areaPerGround = area/groundArea;
}

void ShelterFace::update() {
    SurfaceRadiation rad = (*pCoverSurfaceRadiation);
    if (_hasScreens) rad *= (*pScreensSurfaceRadiation);
    screensMaxState = 0.;
    for (Box *screen : _screens) {
        double state = screen->port("state")->value<double>();
        if (state > screensMaxState) screensMaxState = state;
    }
    set(rad);
    updateU();
    updateHaze();
    updateAirTransmission();
    updateLightTransmission();
}

void ShelterFace::updateU() {
    if (_hasScreens && *pScreensU < phys_math::infinity()) {
        double resistance = 1./(*pCoverU) + 1./(*pScreensU*screensMaxState);
        U = 1./resistance;
    }
    else
        U = *pCoverU;
}

void ShelterFace::updateHaze() {
    if (_hasScreens) {
        double unhazed = (1 - (*pCoverHaze)) * (1 - (*pScreensHaze)*screensMaxState);
        haze = 1 - unhazed;
    }
    else
        haze = *pCoverHaze;
}

void ShelterFace::updateAirTransmission() {
    airTransmissivity = _hasScreens ? *pScreensAirTransmission*screensMaxState : 1.;
}

void ShelterFace::updateLightTransmission() {
    diffuseLightTransmitted = relativeArea*outdoorsDiffuseRadiation * (*face.diffuse.tra);
    double directLightTransmittedTotal = relativeArea*outdoorsDirectRadiation * (*face.direct.tra);
    directLightTransmitted = directLightTransmittedTotal*(1-haze);
    diffuseLightTransmitted += directLightTransmittedTotal*haze;
    totalLightTransmitted = diffuseLightTransmitted + directLightTransmitted;

    double diffuseLightAbsorbedTotal =  relativeArea*outdoorsDiffuseRadiation * (*face.diffuse.abs);
    double directLightAbsorbedTotal =  relativeArea*outdoorsDirectRadiation * (*face.direct.abs);
    double diffuseAbsSum = (*cover.diffuse.abs);
    double directAbsSum  = (*cover.direct.abs);
    if (_hasScreens) {
        diffuseAbsSum += (*screens.diffuse.abs*screensMaxState);
        directAbsSum  += (*screens.direct.abs*screensMaxState);
    }
    lightAbsorbedCover = div0(*cover.diffuse.abs, diffuseAbsSum) * diffuseLightAbsorbedTotal +
                         div0(*cover.direct.abs, directAbsSum) * directLightAbsorbedTotal;
    lightAbsorbedScreens = _hasScreens ?
                           div0(*screens.diffuse.abs, diffuseAbsSum) * diffuseLightAbsorbedTotal +
                           div0(*screens.direct.abs, directAbsSum) * directLightAbsorbedTotal :
                           0.;
}

} //namespace

