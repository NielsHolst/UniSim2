/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_ENERGY_FLUX_BASE_H
#define VG_ENERGY_FLUX_BASE_H

#include <QVector>
#include <base/box.h>

namespace vg {

class EnergyFluxBase : public base::Box
{
public:
    EnergyFluxBase(QString name, QObject *parent);
protected:
    double value;
};
} //namespace


#endif
