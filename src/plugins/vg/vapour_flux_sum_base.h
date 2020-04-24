/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VAPOUR_FLUX_SUM_BASE_H
#define VAPOUR_FLUX_SUM_BASE_H

#include "vapour_flux_base.h"

namespace vg {

class VapourFluxSumBase : public VapourFluxBase
{
public:
    VapourFluxSumBase(QString name, QObject *parent);
    void initialize();
    void update();
private:
    // Data
    struct Ptr {
        const double *conductance, *vapourFlux, *gain;
    };
    QVector<Ptr> ptrs;
    // Methods
    virtual QList<VapourFluxBase*> fluxes() = 0;
};
} //namespace


#endif
