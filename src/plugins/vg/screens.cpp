/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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

#include <base/dialog.h>

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
    Exception::setContext(this);
//    dialog().information("Screens::update() A");
    for (ScreenInfo si: screenInfos) {
        QString s = "(%1 %2 %3)";
//        dialog().information(s.arg(*si.transmissivityLightNet).arg(*si.absorptivityLwOuterNet).arg(*si.absorptivityLwInnerNet));
        SurfaceRadiation &screenRad( SurfaceRadiation().asScreen(*si.transmissivityLightNet, *si.absorptivityLwOuterNet, *si.absorptivityLwInnerNet) );
//        dialog().information("Screens::update() B");
        rad *= screenRad;
//        dialog().information("Screens::update() C");
    }
//    dialog().information("Screens::update() Y");
    set(rad);
//    dialog().information("Screens::update() Z");
}

} //namespace

