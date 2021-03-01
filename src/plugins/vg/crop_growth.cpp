/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "crop_growth.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(CropGrowth)

CropGrowth::CropGrowth(QString name, QObject *parent)
	: Box(name, parent)
{
    help("computes crop growth and energy budget");
    Input(Pn).imports("../photosynthesis[Pn]",CA);
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(netGrowthRate).help("Net rate of biomass growth").unit("g/m2/h");
    Output(netGrowthAllocation).help("Biomass allocated in this time step").unit("g/m2");
}

void CropGrowth::update() {
    double dt = 3600*timeStepSecs;
    netGrowthRate = Pn*30./44.;  // g CH20 from g CO2
    netGrowthAllocation = netGrowthRate*dt;
}

} //namespace

