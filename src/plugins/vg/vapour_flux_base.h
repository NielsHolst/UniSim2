/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_VAPOUR_FLUX_BASE_H
#define VG_VAPOUR_FLUX_BASE_H

#include <base/box.h>

namespace vg {

class VapourFluxBase : public base::Box
{
public:
    VapourFluxBase(QString name, QObject *parent);
protected:
    // Outputs
    double conductance, gain, vapourFlux;
};
} //namespace


#endif
