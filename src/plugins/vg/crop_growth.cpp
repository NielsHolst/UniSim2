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
    Input(respRoot).equals(0.03).help("Root base respiration at 25 oC [g CH2O/g/d]");
    Input(respStem).equals(0.03).help("Stem base respiration at 25 oC [g CH2O/g/d]");
    Input(respLeaf).equals(0.015).help("Leaf base respiration at 25 oC [g CH2O/g/d]");
    Input(respFruit).equals(0.01).help("Fruit base respiration at 25 oC [g CH2O/g/d]");
    Input(massRoot).imports("../mass[root]");
    Input(massStem).imports("../mass[stem]");
    Input(massLeaf).imports("../mass[leaf]");
    Input(massFruit).imports("../mass[fruit]");
    Input(Tcrop).imports("../temperature[value]");
    Input(Pg).imports("./Pg[value]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(maintenanceRespiration).help("Mass lost to base respiration in this time step [g/m2]");
    Output(grossGrowthRate).help("Mass allocated to growth and growth respiration in this time step [g/m2]");
}

void CropGrowth::update() {
    const double Q10 = 2.;
    double dt = timeStep/3600; // [h]
    maintenanceRespiration = maintenanceRespirationRate()*dt*pow(Q10, 0.1*(Tcrop-25.));
    grossGrowthRate = max(Pg*dt*30./44. - maintenanceRespiration, 0.);
}

double CropGrowth::maintenanceRespirationRate() {
    // At 25 oC [g CH2O/d]
    double rate = respRoot*massRoot +
                  respStem*massStem +
                  respLeaf*massLeaf +
                  respFruit*massFruit;
    // At 25 oC [g CH2O/h]
    return rate/24.;

}

} //namespace

