/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/general.h>
#include <base/path.h>
#include <base/publish.h>
#include "vapour_flux_sum.h"

using namespace base;

namespace vg {

PUBLISH(VapourFluxSum)

VapourFluxSum::VapourFluxSum(QString name, QObject *parent)
    : VapourFluxSumBase(name, parent)
{
    help("computes sum of vapour fluxes (children and additional)");
    Input(toAdd).help("Names of boxes to add in addition to children");
}

QList<VapourFluxBase*> VapourFluxSum::fluxes() {
    return childFluxes() + referredFluxes();
}

QList<VapourFluxBase*> VapourFluxSum::childFluxes() {
    return Path("./*", this).resolveMany<VapourFluxBase>().toList();
}

QList<VapourFluxBase*> VapourFluxSum::referredFluxes() {
    QList<VapourFluxBase*> models;
    for (auto modelName : toAdd) {
        auto flux = Path(modelName).resolveOne<VapourFluxBase>(this);
        models << flux;
    }
    return models;
}

} //namespace

