/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/box_builder.h>
#include <base/path.h>
#include <base/publish.h>
#include "cover.h"
#include "general.h"
#include "screens.h"
#include "shelter_face.h"

using namespace base;

namespace vg {

PUBLISH(ShelterFace)

/*! \class ShelterFace
 * \brief A shelter face consists of the cover and any screens
 *
 * Inputs
 * ------
 * - _roofArea_ is the total area of the roof (the two sloping surfaces on top of each span) [m<SUP>2</SUP>]
 * - _sideWallsArea_ is the total area of the two greenhouse side walls (facing the outside) [m<SUP>2</SUP>]
 * - _endWallsArea_ is the total area of the two greenhouse end walls (excluding the triangular gables) [m<SUP>2</SUP>]
 * - _gablesArea_ is the total area of the two triangular gables at the ends of each span [m<SUP>2</SUP>]
 * - _groundArea_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 * - _outdoorsDirectRadiation_ is the direct component of sunlight irradiation [W/m<SUP>2</SUP>]
 * - _outdoorsDiffuseRadiation_ is the diffuse component of sunlight irradiation [W/m<SUP>2</SUP>]
 *
 * Outputs
 * -------
 * - _area_ is the surface area [m<SUP>2</SUP>]
 * - _relativeArea_ is the proportion of this surface out of the total surface area [0;1]
 * - _areaPerGround_ is the surface area relative to the ground area [m<SUP>2</SUP> shelter/m<SUP>2</SUP> ground]
 */

ShelterFace::ShelterFace(QString name, QObject *parent)
    : ShelterBase(name, parent)
{
    Input(roofArea).imports("geometry[roofArea]");
    Input(sideWallsArea).imports("geometry[sideWallsArea]");
    Input(endWallsArea).imports("geometry[endWallsArea]");
    Input(gablesArea).imports("geometry[gablesArea]");
    Input(groundArea).imports("geometry[groundArea]");

    Input(outdoorsDirectRadiation).imports("outdoors[directRadiation]");
    Input(outdoorsDiffuseRadiation).imports("outdoors[diffuseRadiation]");

    Output(area);
    Output(relativeArea);
    Output(areaPerGround);
    Output(screensMaxState);
}

void ShelterFace::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./cover"))
        builder.box("Cover").name("cover").endbox();
//    if (!findMaybeOne<Box>("./screens"))
//        builder.box("Screens").name("screens").endbox();
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
    if (_hasScreens) {
        double resistance = 1./(*pCoverU) + 1./(*pScreensU);
        U = 1./resistance;
    }
    else
        U = *pCoverU;
}

void ShelterFace::updateHaze() {
    if (_hasScreens) {
        double unhazed = (1 - (*pCoverHaze)) * (1 - (*pScreensHaze));
        haze = 1 - unhazed;
    }
    else
        haze = *pCoverHaze;
}

void ShelterFace::updateAirTransmission() {
    airTransmissivity = _hasScreens ? *pScreensAirTransmission : 1.;
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
        diffuseAbsSum += (*screens.diffuse.abs);
        directAbsSum  += (*screens.direct.abs);
    }
    lightAbsorbedCover = div0(*cover.diffuse.abs, diffuseAbsSum) * diffuseLightAbsorbedTotal +
                         div0(*cover.direct.abs, directAbsSum) * directLightAbsorbedTotal;
    lightAbsorbedScreens = _hasScreens ?
                           div0(*screens.diffuse.abs, diffuseAbsSum) * diffuseLightAbsorbedTotal +
                           div0(*screens.direct.abs, directAbsSum) * directLightAbsorbedTotal :
                           0.;
}

} //namespace

