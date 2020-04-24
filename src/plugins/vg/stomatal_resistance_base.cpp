/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include "stomatal_resistance_base.h"

using namespace base;
using namespace phys_math;

namespace vg {

StomatalResistanceBase::StomatalResistanceBase(QString name, QObject *parent)
	: Box(name, parent)
{
    Class(StomatalResistanceBase);
    help("base class for computing stomatal resistance");
    Output(rsH2O).help("Stomatal resistance against water vapour").unit("s/m");
    Output(rsCo2).help("Stomatal resistance against CO2").unit("s/m");
}

void StomatalResistanceBase::reset() {
    rsH2O = resetRsH2O();
    rsCo2 = rsH2O*1.6;
}

void StomatalResistanceBase::update() {
    rsH2O = updateRsH2O();
    rsCo2 = rsH2O*1.6;
}

} //namespace

