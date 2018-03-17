/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <base/box_builder.h>
#include <base/path.h>
#include <base/publish.h>
#include "shelter.h"
#include "surface_radiation.h"

using std::max;
using namespace base;

namespace vg {

PUBLISH(Shelter)

/*! \class Shelter
 * \brief Collection of all greenhouse shelter faces
 *
 * Inputs
 * ------
 * - _groundArea_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _heatCapacityCoversPerGround_ is the total heat capacity of all covers [J/kg/m<SUP>2</SUP> ground]
 * - _heatCapacityScreensPerGround_ is the total heat capacity of all screens [J/kg/m<SUP>2</SUP> ground]
 * - _screensEffectiveArea_ is the total drawn of screens [m<SUP>2</SUP>]
 * - _screensPerGroundArea_ is the _screensEffectiveArea_ per _ground area [m<SUP>2</SUP> screen/m<SUP>2</SUP> ground]
 * - _screensMaxState_ is maximum state of all screens [0;1]
 */

Shelter::Shelter(QString name, QObject *parent)
    : ShelterBase(name, parent)
{
    Input(groundArea).imports("geometry[groundArea]");
    Output(heatCapacityCoversPerGround);
    Output(heatCapacityScreensPerGround);
    Output(screensEffectiveArea);
    Output(screensPerGroundArea);
    Output(horizontalScreenState);
    Output(hasHorizontalScreen);
    Output(ventsEffectiveArea).imports("*/vent[effectiveArea]").transform(Sum).help("Effective area of vents (m2)");
    Output(ventsMaxEffectiveArea).imports("*/vent[maxEffectiveArea]").transform(Sum).help("Effective area of vents when max. open (m2)");
}

void Shelter::amend() {
    BoxBuilder builder(this);
    amendShelter(builder, "roof1");
    amendShelter(builder, "roof2");
    amendShelter(builder, "side1");
    amendShelter(builder, "side2");
    amendShelter(builder, "end1");
    amendShelter(builder, "end2");
}

void Shelter::amendShelter(BoxBuilder &builder, QString shelterName) {
    if (!findMaybeOne<Box>("./" + shelterName))
        builder.box("ShelterFace").name(shelterName).endbox();
}

#define Pull(p) info.p = face->port(#p)->valuePtr<double>()

void Shelter::initialize() {
    infos.clear();
    QVector<ShelterBase*> faces = findMany<ShelterBase>("./*");
    for (ShelterBase* face : faces) {
        FaceInfo info;
        info.sr = face->surfaceRadiation();
        Pull(diffuseLightTransmitted);
        Pull(directLightTransmitted);
        Pull(totalLightTransmitted);
        Pull(lightAbsorbedCover);
        Pull(lightAbsorbedScreens);
        Pull(haze);
        Pull(U);
        Pull(airTransmissivity);
        Pull(screensMaxState);
        Pull(area);
        Pull(relativeArea);
        Box *cover   = face->findOne<Box>("./cover"),
            *screens = face->findMaybeOne<Box>("./screens");
        info.heatCapacityCover = cover->port("heatCapacity")->valuePtr<double>();
        info.heatCapacityScreens = screens ? screens->port("heatCapacity")->valuePtr<double>() : 0;
        info.screensEffectiveArea = screens ? screens->port("effectiveArea")->valuePtr<double>() : 0;
        infos << info;
    }
    initHorizontalScreens();
}

void Shelter::initHorizontalScreens() {
    QVector<Box*> roofScreens = findMany<Box>("./roof1/screens/*|./roof2/screens/* ");
    for (Box *screen : roofScreens) {
        if (screen->port("isHorizontal")->value<bool>())
            _horizontalScreens << screen;
    }
    hasHorizontalScreen = !_horizontalScreens.isEmpty();
}

#define Accumulate(p) if (info.p) p += (*info.p);
#define AccumulateWeighted(p) p += (*info.p) * (*info.relativeArea);
#define AccumulateSr(p) sr.p += info.sr->p * (*info.relativeArea)

void Shelter::update() {
    SurfaceRadiation sr;
    sr.setToZero();
    diffuseLightTransmitted =
    directLightTransmitted =
    totalLightTransmitted =
    lightAbsorbedCover =
    lightAbsorbedScreens =
    U =
    airTransmissivity =
    screensEffectiveArea =
    screensMaxState = 0.;
    double heatCapacityCover{0},
           heatCapacityScreens{0};
    for (FaceInfo info : infos) {
        AccumulateSr(light.inner.abs);
        AccumulateSr(light.inner.ref);
        AccumulateSr(light.outer.abs);
        AccumulateSr(light.outer.ref);
        AccumulateSr(light.tra);
        AccumulateSr(directLight.inner.abs);
        AccumulateSr(directLight.inner.ref);
        AccumulateSr(directLight.outer.abs);
        AccumulateSr(directLight.outer.ref);
        AccumulateSr(directLight.tra);
        AccumulateSr(lw.inner.abs);
        AccumulateSr(lw.inner.ref);
        AccumulateSr(lw.outer.abs);
        AccumulateSr(lw.outer.ref);
        AccumulateSr(lw.tra);

        Accumulate(diffuseLightTransmitted);
        Accumulate(directLightTransmitted);
        Accumulate(totalLightTransmitted);
        Accumulate(lightAbsorbedCover);
        Accumulate(lightAbsorbedScreens);
        AccumulateWeighted(haze);
        AccumulateWeighted(U);
        AccumulateWeighted(airTransmissivity);
        Accumulate(heatCapacityCover);
        Accumulate(heatCapacityScreens);
        Accumulate(screensEffectiveArea);

        if (*info.screensMaxState > screensMaxState)
            screensMaxState = *info.screensMaxState;
    }
    heatCapacityCoversPerGround = heatCapacityCover/groundArea;
    heatCapacityScreensPerGround = heatCapacityScreens/groundArea;
    screensPerGroundArea = screensEffectiveArea/groundArea;
    set(sr);

    horizontalScreenState = 0.;
    for (Box *screen : _horizontalScreens) {
        double state = screen->port("state")->value<double>();
        if (state > horizontalScreenState)
            horizontalScreenState = state;
    }
}

} //namespace

