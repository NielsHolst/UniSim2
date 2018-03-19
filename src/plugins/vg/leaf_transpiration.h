/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_LEAF_TRANSPIRATION_H
#define VG_LEAF_TRANSPIRATION_H

#include <base/box.h>
#include "vapour_flux_base.h"

namespace vg {

class LeafTranspiration : public VapourFluxBase
{
public:
    LeafTranspiration(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double radiationAbsorbed, rsH2O, rbH2O,
        indoorsAh, leafTemperature, lai, fractionPlantArea;
    // Outputs
    double leafAh;
};
} //namespace


#endif
