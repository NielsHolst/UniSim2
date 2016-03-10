/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
