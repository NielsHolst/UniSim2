/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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

Shelter::Shelter(QString name, QObject *parent)
    : ShelterBase(name, parent)
{
    help("models the greenhouse sjeler");
    Input(groundArea).imports("geometry[groundArea]").unit("m2");
    Output(heatCapacityCoversPerGround).help("Capacity of all covers").unit("J/ground m2");
    Output(heatCapacityScreensPerGround).help("Capacity of all screens").unit("J/ground m2");
    Output(screensEffectiveArea).help("Effective (i.e., drawn) area of screens").unit("m2");
    Output(screensPerGroundArea).help("Effective screen area per ground area").unit("m2/m2");
    Output(screensMaxState).help("Max. state of all screens").unit("[0;1]");
    Output(horizontalScreenState).help("State of any horizontal screens [0;1]").unit("[0;1]");
    Output(hasHorizontalScreen).help("Has the greenhouse got horizontal screens?").unit("y|n");
//    Output(ventsEffectiveArea).imports("*/vent[effectiveArea]").transform(Sum)
//            .help("Effective area of vents [m2]");
//    Output(ventsMaxEffectiveArea).imports("*/vent[maxEffectiveArea]").transform(Sum)
//            .help("Effective area of vents when max. open [m2]");
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
        info.heatCapacityScreens = screens ? screens->port("heatCapacity")->valuePtr<double>() : nullptr;
        info.screensEffectiveArea = screens ? screens->port("effectiveArea")->valuePtr<double>() : nullptr;
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

