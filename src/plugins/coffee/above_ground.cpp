/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "above_ground.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(AboveGround)

AboveGround::AboveGround(QString name, QObject *parent)
    : Box(name, parent) {
    help("models light interception");
    Input(dayLength).imports("sun[dayLength]");
    Input(globRad).imports("weather[GlobRad]");
    Input(kCoffee).imports("param[kExt]");
    Input(kTree).imports("param[kExtT]");
    Input(laiCoffee).imports("/*/coffee[lai]");
    Input(laiTree).imports("param[laiTree]");
    Output(globRadShade).unit("MJ/m2/d").help("Sunlight intensity under the crown");
    Output(parShade).unit("MJ PAR/m2/d").help("PAR intensity under the crown");
    Output(parShadeAvg).unit("MJ PAR/m2/h").help("Average light intensity during photoperiod");
    Output(parInterceptedCoffee).unit("MJ PAR/m2/d").help("Total light interception by coffee");
}

void AboveGround::update() {
    // Sunlight under the crown
    globRadShade = globRad*exp(-kTree * laiTree);
    // PAR is half of solar radiation
    parShade = 0.5*globRadShade;
    // PAR intercepted by coffee
    parShadeAvg = parShade/dayLength;
    parInterceptedCoffee = parShade*(1. - exp(-kCoffee*laiCoffee));
}

} //namespace

