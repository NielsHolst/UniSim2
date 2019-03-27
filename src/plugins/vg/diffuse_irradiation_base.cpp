/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "diffuse_irradiation_base.h"
using namespace base;

namespace vg {


DiffuseIrradiationBase::DiffuseIrradiationBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(DiffuseIrradiationBase);
    help("computes diffuse fraction of global irradiation");
    Input(angot).imports("calendar[angot]").unit("W/m2");
    Input(globalIrradiation).imports("outdoors[radiation]").unit("W/m2");
    Output(clearness).help("Fraction of angot irradiation").unit("[0;1]");
}

void DiffuseIrradiationBase::updateClearness() {
    clearness = (angot > 0.) ? std::min(globalIrradiation/angot, 1.) : 0.;
}

} //namespace

