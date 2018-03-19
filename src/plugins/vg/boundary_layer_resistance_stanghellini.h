/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_BOUNDARY_LAYER_RESISTANCE_STANGHELLINI_H
#define VG_BOUNDARY_LAYER_RESISTANCE_STANGHELLINI_H

#include "boundary_layer_resistance_base.h"

namespace vg {

class BoundaryLayerResistanceStanghellini : public BoundaryLayerResistanceBase
{
public:
    BoundaryLayerResistanceStanghellini(QString name, QObject *parent);
    void setRbH2O();
    // Inputs
    double leafTemperature, indoorsTemperature;
};
} //namespace


#endif
