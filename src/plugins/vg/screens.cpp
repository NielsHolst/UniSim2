/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/exception.h>
#include <base/path.h>
#include <base/test_num.h>
#include "general.h"
#include <base/publish.h>
#include "screens.h"
#include "surface_radiation.h"


using std::max;
using namespace base;

namespace vg {

PUBLISH(Screens)

Screens::Screens(QString name, QObject *parent)
    : SurfaceRadiationOutputs(name, parent)
{
    help("summarizes a colletion of Screen models");
    Output(areHorizontal).help("Are these screens horizontal?").unit("y|n");
    Output(maxState).help("Maximum screen state").unit("[0;1]");
    Output(airTransmissivity).help("Proportion of air transmitted through the screens").unit("[0;1]");
    Output(haze).help("Proportion of direct light becoming dispersed on passage through the screens").unit("[0;1]");
    Output(U).help("Combined U-value of all screens").unit("W/K/m2");
    Output(heatCapacity).help("Combined heat capacity of all screens").unit("J/K");
    Output(effectiveArea).help("The drawn area of the screens").unit("m2");
}

void Screens::initialize() {
    QVector<Box*> screens = Path("./*", this).resolveMany<Box>();
    screenInfos = collectScreenInfos(screens);
}

QVector<Screens::ScreenInfo> Screens::collectScreenInfos(QVector<Box*> screenModels) {
    QVector<Screens::ScreenInfo> screenInfos;
    for (Box *screen: screenModels) {
        screenInfos << ScreenInfo {
            screen->port("isHorizontal")->valuePtr<bool>(),
            screen->port("transmissivityLightNet")->valuePtr<double>(),
            screen->port("absorptivityLwInnerNet")->valuePtr<double>(),
            screen->port("absorptivityLwOuterNet")->valuePtr<double>(),
            screen->port("state")->valuePtr<double>(),
            screen->port("unhazed")->valuePtr<double>(),
            screen->port("transmissivityAirNet")->valuePtr<double>(),
            screen->port("resistance")->valuePtr<double>(),
            screen->port("heatCapacity")->valuePtr<double>(),
            screen->port("effectiveArea")->valuePtr<double>()
        };
    }
    return screenInfos;
}

void Screens::reset() {
    areHorizontal = false;
    for (ScreenInfo info: screenInfos)
        areHorizontal = areHorizontal || (*info.isHorizontal);
    resetRadiationOutputs();
    maxState = haze = 0;
    airTransmissivity = 1;
    U = infinity();
    heatCapacity = effectiveArea = 0.;
}

void Screens::update() {
    maxState = 0;
    airTransmissivity = 1;
    heatCapacity =
    effectiveArea  = 0;
    double resistance{0}, unhazed{1};
    for (ScreenInfo info: screenInfos) {
        maxState = max(maxState, *info.state);
        airTransmissivity *= *info.airTransmissionNet;
        unhazed *= *info.unhazed;
        resistance += *info.resistance;
        heatCapacity += *info.heatCapacity;
        effectiveArea += *info.effectiveArea;
    }
    TestNum::snapToZero(maxState);
    haze = 1. - unhazed;
    U = 1./resistance;

    SurfaceRadiation rad;
    for (ScreenInfo si: screenInfos) {
        SurfaceRadiation &screenRad( SurfaceRadiation().asScreen(*si.transmissivityLightNet, *si.absorptivityLwOuterNet, *si.absorptivityLwInnerNet) );
        rad *= screenRad;
    }
    set(rad);
}

} //namespace

