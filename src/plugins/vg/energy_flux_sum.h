/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_ENERGY_FLUX_SUM_H
#define VG_ENERGY_FLUX_SUM_H

#include <QVector>
#include "energy_flux_base.h"

namespace vg {

class EnergyFluxSum : public EnergyFluxBase
{
public:
    EnergyFluxSum(QString name, QObject *parent);
    void initialize();
    void update();

private:
    QVector<const double *> fluxes;
};
} //namespace


#endif
