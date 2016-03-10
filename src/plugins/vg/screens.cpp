/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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


/*! \class Screens
 * \brief A collection of Screen objects
 *
 * Outputs
 * ------
 * - _areHorizontal_ tells if these screens are horizontal [true/false]
 * - _maxState_ is the maximum screen state [0;1]
 * - _airTransmissivity_ is the proportion of air transmitted through the screens [0;1]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the screens [0;1]
 * - _U_ is the combined U-value of all screens [W/m<SUP>2</SUP>/K]
 * - _heatCapacity_ is the heat capacity [J/K]
 * - _effectiveArea_ is the draw area of the screens
 */

Screens::Screens(QString name, QObject *parent)
    : SurfaceRadiationOutputs(name, parent)
{
    Output(areHorizontal);
    Output(maxState);
    Output(airTransmissivity);
    Output(haze);
    Output(U);
    Output(heatCapacity);
    Output(effectiveArea);
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

