/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "crop_growth.h"
#include <base/publish.h>

using namespace std;
using namespace base;

namespace vg {

PUBLISH(CropGrowth)

/*! \class CropGrowth
 * \brief Crop growth rate and energy budget
 *
 * Inputs
 * ------
 * - _respRoot_ is the root basic respiration at 25 <SUP>o</SUP>C [g CH<SUB>2</SUB>O/g/d]
 * - _respStem_ is the stem basic respiration at 25 <SUP>o</SUP>C [g CH<SUB>2</SUB>O/g/d]
 * - _respLeaf_ is the leaf basic respiration at 25 <SUP>o</SUP>C [g CH<SUB>2</SUB>O/g/d]
 * - _respFruit_ is the fruit basic respiration at 25 <SUP>o</SUP>C [g CH<SUB>2</SUB>O/g/d]
 * - _massRoot_ is root dry mass [g/m<SUP>2</SUP>]
 * - _massStem_ is stem dry mass [g/m<SUP>2</SUP>]
 * - _massLeaf_ is leaf dry mass [g/m<SUP>2</SUP>]
 * - _massFruit_ is fruit dry mass [g/m<SUP>2</SUP>]
 * - _Tcrop_ is crop temperature [<SUP>o</SUP>C]
 * - _Pg_ is crop assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP>/h]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _maintenanceRespiration_ is the mass lost to maintenance in this time step [g/m<SUP>2</SUP>]
 * - _grossGrowthRate_ is the mass allocated to growth and growth respiration in this time step [g/m<SUP>2</SUP>]
 */

CropGrowth::CropGrowth(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(respRoot, 0.03);
    Input(respStem, 0.03);
    Input(respLeaf, 0.015);
    Input(respFruit, 0.01);
    Input(massRoot, "../mass[root]");
    Input(massStem, "../mass[stem]");
    Input(massLeaf, "../mass[leaf]");
    Input(massFruit, "../mass[fruit]");
    Input(Tcrop, "../temperature[value]");
    Input(Pg, "./Pg[value]");
    Input(timeStep, "calendar[timeStepSecs]");

    Output(maintenanceRespiration);
    Output(grossGrowthRate);
}

void CropGrowth::reset() {
    maintenanceRespiration = grossGrowthRate = 0.;
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

