/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VAPOUR_FLUX_SUM_H
#define VAPOUR_FLUX_SUM_H

#include "vapour_flux_sum_base.h"

namespace vg {

class VapourFluxSum : public VapourFluxSumBase
{
public:
    VapourFluxSum(QString name, QObject *parent);
private:
    // Input
    QVector<QString> toAdd;
    // Methods
    QList<VapourFluxBase*> fluxes();
    QList<VapourFluxBase*> childFluxes();
    QList<VapourFluxBase*> referredFluxes();
};
} //namespace


#endif
