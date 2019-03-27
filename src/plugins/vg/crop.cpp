/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "crop.h"
#include "general.h"
#include <base/box_builder.h>
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(Crop)

Crop::Crop(QString name, QObject *parent)
	: Box(name, parent)
{
    help("holds crop parameters");
    Input(density).equals(3.9).unit("/m2");
}

void Crop::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./periods"))
        builder.
        box().name("periods").
            box("DateTimeSignal").
                port("circadian").equals(false).
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./physTime"))
        builder.
        box("DayDegrees").name("physTime").
            port("T").imports("indoors/temperature[value]").
            port("timeStepDays").imports("calendar[timeStepDays]").
            port("isTicking").imports("../periods/*[flag]").transform(Any).
            port("resetTotal").imports("../periods/*[flagDown]").transform(Any).
        endbox();

    if (!findMaybeOne<Box>("./lai"))
        builder.
        box("vg::FruitCropLai").name("lai").
            port("laiStartPerPlant").equals(0.385).
            port("maxLai").equals(3.5).
            port("fractionPlantArea").equals(0.9).
        endbox();

    if (!findMaybeOne<Box>("./layers"))
        builder.
        box().name("layers").
        endbox();

    amendLayer("top");
    amendLayer("middle");
    amendLayer("bottom");

    if (!findMaybeOne<Box>("./radiation"))
        builder.
        box("vg::CropRadiation").name("radiation").
        endbox();
    if (!findMaybeOne<Box>("./temperature"))
        builder.
        box("vg::Average").name("temperature").
            port("inputs").equals("../layers/*/temperature[value]").
        endbox();
    if (!findMaybeOne<Box>("./lightAbsorbed"))
        builder.
        box("vg::Sum").name("lightAbsorbed").
            port("inputs").equals("../layers/*/radiationAbsorbed[lightAbsorbed]").
        endbox();
    if (!findMaybeOne<Box>("./heatingAbsorbed"))
        builder.
        box("vg::Sum").name("heatingAbsorbed").
            port("inputs").equals("../layers/*/radiationAbsorbed[heatingAbsorbed]").
        endbox();
    if (!findMaybeOne<Box>("./growthLightLwAbsorbed"))
        builder.
        box("vg::Sum").name("growthLightLwAbsorbed").
            port("inputs").equals("../layers/*/radiationAbsorbed[growthLightLwAbsorbed]").
        endbox();
    if (!findMaybeOne<Box>("./radiationAbsorbed"))
        builder.
        box("vg::Sum").name("radiationAbsorbed").
            port("inputs").equals("../layers/*/radiationAbsorbed[lightAbsorbed]").
        endbox();
    if (!findMaybeOne<Box>("./conductance"))
        builder.
        box("vg::Average").name("conductance").
            port("inputs").equals("../layers/*/transpiration[conductance]").
        endbox();
    if (!findMaybeOne<Box>("./vapourFlux"))
        builder.
        box("vg::Average").name("vapourFlux").
            port("inputs").equals("../layers/*/transpiration[vapourFlux]").
        endbox();
    if (!findMaybeOne<Box>("./gain"))
        builder.
        box("vg::Average").name("gain").
            port("inputs").equals("../layers/*/transpiration[gain]").
        endbox();
    if (!findMaybeOne<Box>("./growth"))
        builder.
        box("vg::CropGrowth").name("growth").
            box("vg::Sum").name("Pg").
                port("inputs").equals("../../layers/*/photosynthesis[Pg]").
            endbox().
        endbox();
    if (!findMaybeOne<Box>("./mass"))
        builder.
        box("vg::FruitCropMass").name("mass").
            port("tempSumReproductive").equals(900).
            port("tempMaxReproductive").equals(45).
            port("vegPropRoot").equals(0).
            port("vegPropStem").equals(0.4).
            port("vegPropLeaf").equals(0.6).
            port("vegPropFruit").equals(0).
            port("reproPropRoot").equals(0).
            port("reproPropStem").equals(0.05).
            port("reproPropLeaf").equals(0.1).
            port("reproPropFruit").equals(0.85).
            port("fruitFactor").imports("./fruitFactor[value]").
            box("vg::FruitFactor").name("fruitFactor").
                port("minDensity").equals(3.9).
                port("maxValue").equals(1.044).
            endbox().
        endbox();
    if (!findMaybeOne<Box>("./yield"))
        builder.
        box("vg::CropYield").name("yield").
            port("fractionDryWeight").equals(0.045).
            port("produceMass").imports("../mass[fruit]").
        endbox();

}

void Crop::amendLayer(QString level) {
    BoxBuilder builder(findOne<Box>("./layers"));
    if (!findMaybeOne<Box>("./layers/"+level))
        builder.
        box("vg::LeafLayer").name(level).
            box("LeafWindSpeed").name("windSpeed").
                port("k").equals(0.6).
            endbox().
            box("vg::StomatalResistanceRose").name("rs").
            endbox().
            box("vg::BoundaryLayerResistanceStanghellini").name("rb").
            endbox().
            box("vg::LeafRadiationAbsorbed").name("radiationAbsorbed").
            endbox().
            box("vg::LeafTranspiration").name("transpiration").
            endbox().
            box("vg::LeafTemperature").name("temperature").
            endbox().
            box("vg::LeafPhotosynthesis").name("photosynthesis").
                box("LeafLightResponse").name("lightResponse").
                endbox().
            endbox().
        endbox();
}

} //namespace

