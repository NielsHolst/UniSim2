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
    Input(respRoot).equals(0.03).help("Root base respiration at 25 oC").unit("g CH2O/g/d");
    Input(respStem).equals(0.03).help("Stem base respiration at 25 oC").unit("g CH2O/g/d");
    Input(respLeaf).equals(0.015).help("Leaf base respiration at 25 oC").unit("g CH2O/g/d");
    Input(respFruit).equals(0.01).help("Fruit base respiration at 25 oC").unit("g CH2O/g/d");
    Input(massRoot).imports("../mass[root]").unit("g/m2");
    Input(massStem).imports("../mass[stem]").unit("g/m2");
    Input(massLeaf).imports("../mass[leaf]").unit("g/m2");
    Input(massFruit).imports("../mass[fruit]").unit("g/m2");
    Input(Tcrop).imports("../temperature[value]").unit("oC");
    Input(Pg).imports("./Pg[value]").unit("g CO2/m2/h");
    Input(timeStepSecs).imports("calendar[timeStepSecs]").unit("s");
    Output(grossGrowthRate).help("Gross rate of mass fixated (growth + respiration)").unit("g/m2/h");
    Output(netGrowthRate).help("Net rate of growth").unit("g/m2/h");
    Output(maintenanceRespirationRate).help("Respiration rate").unit("g/m2/h");
    Output(netGrowthAllocation).help("Mass allocated to growth in this time step[g/m2]").unit("g/m2");
}

void CropGrowth::update() {
    const double Q10 = 2.;
    double dt = timeStepSecs/3600; // [h]
    maintenanceRespirationRate = _maintenanceRespirationRate()*pow(Q10, 0.1*(Tcrop-25.));
    netGrowthRate = Pg*30./44.;                                     // Swap net and gross?
    grossGrowthRate = netGrowthRate - maintenanceRespirationRate;
    netGrowthAllocation = netGrowthRate*dt;
}

double CropGrowth::_maintenanceRespirationRate() {
    // At 25 oC [g CH2O/d]
    double rate = respRoot*massRoot +
                  respStem*massStem +
                  respLeaf*massLeaf +
                  respFruit*massFruit;
    // At 25 oC [g CH2O/h]
    return rate/24.;

}

} //namespace

