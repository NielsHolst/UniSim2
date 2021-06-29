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
    Input(Pn).imports("../photosynthesis[Pn]",CA);                   // g CO2   /m2 ground/h
    Input(parAbsorbed).imports("energyBudget/crop[parAbsorbed]",CA); // μmol PAR/m2 ground/s
    Output(netGrowthRate).help("Net rate of biomass growth").unit("g/m2/h");
    Output(lue).help("Light use efficiency").unit("g/mol PAR");
}

void CropGrowth::update() {
    netGrowthRate = Pn*30./44.;  // g CH20/m2 ground/h
    lue = (parAbsorbed<1e-3) ? 0. : netGrowthRate/3600. / (parAbsorbed*1e-6);
    if (lue < 0.)
        lue = 0.;
}

} //namespace

