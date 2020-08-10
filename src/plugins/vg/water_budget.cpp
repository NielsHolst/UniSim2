/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "water_budget.h"

using namespace base;

namespace vg {

PUBLISH(WaterBudget)

WaterBudget::WaterBudget(QString name, QObject *parent)
    : Box(name, parent)
{
    help("collects all water vapour fluxes");
    Input(conductance).imports("./*[conductance]");
    Input(vapourFlux).imports("./*[vapourFlux]");
    Input(gain).imports("./*[gain]");
    Output(conductanceSum).help("Sum of vapour flux conductances").unit("s/m");
    Output(vapourFluxSum).help("Sum of vapour flux rates").unit("kg/m2/s");
    Output(gainSum).help("Sum of vapour flux gains").unit("kg/m2/s");
}

void WaterBudget::amend() {
    bool
        hasScreen1 = !findMany<Box>("construction/shelter/*/screens/layer1").isEmpty(),
        hasScreen2 = !findMany<Box>("construction/shelter/*/screens/layer2").isEmpty(),
        hasScreen3 = !findMany<Box>("construction/shelter/*/screens/layer3").isEmpty();
    BoxBuilder builder(this);

    builder.
    box("VapourFluxVentilation").name("ventilation").
    endbox().
    box("VapourFluxTranspiration").name("transpiration").
    endbox().
    box("VapourFluxCondensationCrop").name("condensationCrop").
    endbox().
    box("VapourFluxCondensationCover").name("condensationCover").
    endbox().
    box().name("condensationScreens").
        newPort("conductance").imports("./*[conductance]").transform(Sum).
        newPort("vapourFlux").imports("./*[vapourFlux]").transform(Sum).
        newPort("gain").imports("./*[gain]").transform(Sum);
        if (hasScreen1)
            builder.
            box("VapourFluxCondensationScreen1").name("screen1").
            endbox();
        if (hasScreen2)
            builder.
            box("VapourFluxCondensationScreen2").name("screen2").
            endbox();
        if (hasScreen3)
            builder.
            box("VapourFluxCondensationScreen3").name("screen3").
            endbox();
    builder.
    endbox();
}

void WaterBudget::reset() {
    update();
}

void WaterBudget::update() {
    conductanceSum = vector_op::sum(conductance);
    vapourFluxSum  = vector_op::sum(vapourFlux);
    gainSum        = vector_op::sum(gain);
}


} //namespace

