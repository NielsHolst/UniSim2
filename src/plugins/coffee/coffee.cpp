/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "coffee.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(Coffee)

Coffee::Coffee(QString name, QObject *parent)
    : Box(name, parent) {
    help("models coffee growth and development");
    Input(isHarvestTime).imports("phenology[isHarvestTime]");
    Input(gLai).imports("foliage[gLai]");
    Input(dLai).imports("senescence[dLai]");
//    Input(dLeafN).imports("senescence[dLeafN]");
    Input(gLeafC).imports("growth[gLeafC]");
    Input(dLeafC).imports("senescence[dLeafC]");
    Input(gWoodyC).imports("growth[gWoodyC]");
    Input(gRootC).imports("growth[gRootC]");
    Input(dRootC).imports("senescence[dRootC]");
    Input(gBerryC).imports("growth[gBerryC]");
    Output(lai);
    Output(leafN);
    Output(leafC);
    Output(woodyC);
    Output(rootC );
    Output(berryC);
    Output(harvestedBerryC);
}

void Coffee::update() {
    lai += gLai - dLai;
//    leafN += gLeafN - dLeafN;
    woodyC += gWoodyC;
    rootC += gRootC - dRootC;
    berryC += gBerryC - harvestedBerryC;
    if (isHarvestTime) {
        harvestedBerryC = berryC;
        berryC = 0.;
    }
    else {
        harvestedBerryC = 0.;
    }
}

} //namespace

