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
    Input(DAYL).imports("sun[dayLength]");
    Input(solarRadiation).imports("weather[solarRadiation]");
    Input(KEXT).equals(0.76).help("Light extinction coefficient of coffee");
    Input(KEXTT).equals(0.7).help("Light extinction coefficient of crown");
    Input(LAI).equals(3.).help("Coffee LAI (replace with CoffeeLAI sub-model output)");
    Input(LAIT).equals(2.).help("Tree LAI  (replace with TreeLAI sub-model output)");
    Output(PARshade).unit("MJ PAR/m2/d").help("Light intensity under the crown");
    Output(PARav).unit("MJ PAR/m2/d").help("Average light intensity during photoperiod");
    Output(PARint).unit("MJ PAR/m2/d").help("Total light interception by coffee");
}

void AboveGround::update() {
    // PAR is half of solar radiation
    double PAR = 0.5*solarRadiation;
    // PAR absorbed by crown
    double PARabsCrown = PAR*(1. - exp(-KEXTT * LAIT));
    // PAR under the crown
    PARshade = PAR - PARabsCrown;
    // PAR intercepted by coffee
    PARav = PARshade/DAYL;
    PARint = PARshade*(1. - exp(-KEXT*LAI));
}

} //namespace

