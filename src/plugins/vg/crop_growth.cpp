/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "crop_growth.h"
#include <base/publish.h>

using namespace std;
using namespace base;

namespace vg {

PUBLISH(CropGrowth)

CropGrowth::CropGrowth(QString name, QObject *parent)
	: Box(name, parent)
{
    help("computes crop growth and energy budget");
//    Input(respRoot).equals(0.03).help("Root base respiration at 25 oC").unit("g CH2O/g/d");
//    Input(respStem).equals(0.03).help("Stem base respiration at 25 oC").unit("g CH2O/g/d");
//    Input(respLeaf).equals(0.015).help("Leaf base respiration at 25 oC").unit("g CH2O/g/d");
//    Input(respFruit).equals(0.01).help("Fruit base respiration at 25 oC").unit("g CH2O/g/d");
//    Input(massRoot).imports("../mass[root]").unit("g/m2");
//    Input(massStem).imports("../mass[stem]").unit("g/m2");
//    Input(massLeaf).imports("../mass[leaf]").unit("g/m2");
//    Input(massFruit).imports("../mass[fruit]").unit("g/m2");
    Input(Pn).imports("../Pn[value]").unit("g CO2/m2/h");
    Input(timeStepSecs).imports("calendar[timeStepSecs]").unit("s");
    Output(netGrowthRate).help("Net rate of biomass growth").unit("g/m2/h");
    Output(netGrowthAllocation).help("Biomass allocated in this time step").unit("g/m2");
}

void CropGrowth::update() {
    double dt = 3600*timeStepSecs;
    netGrowthRate = Pn*30./44.;
    netGrowthAllocation = netGrowthRate*dt;
}

//double CropGrowth::_maintenanceRespirationRate() {
//    // At 25 oC [g CH2O/d]
//    double rate = respRoot*massRoot +
//                  respStem*massStem +
//                  respLeaf*massLeaf +
//                  respFruit*massFruit;
//    // At 25 oC [g CH2O/h]
//    return rate/24.;
//}

} //namespace

