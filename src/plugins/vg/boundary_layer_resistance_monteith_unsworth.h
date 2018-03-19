/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_BOUNDARY_LAYER_RESISTANCE_MONTEITH_UNSWORTH_H
#define VG_BOUNDARY_LAYER_RESISTANCE_MONTEITH_UNSWORTH_H

#include "boundary_layer_resistance_base.h"

namespace vg {

class BoundaryLayerResistanceMonteithUnsworth : public BoundaryLayerResistanceBase
{
public:
    BoundaryLayerResistanceMonteithUnsworth(QString name, QObject *parent);
    void reset();
    void setRbH2O();
private:
    // Inputs
    double leafTemperature, indoorsTemperature;
    // Outputs
    double gbH2Ofree, gbH2Oforced;
};

} //namespace


#endif
