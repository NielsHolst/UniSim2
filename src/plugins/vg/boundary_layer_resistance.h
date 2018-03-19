/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_BOUNDARY_LAYER_RESISTANCE_H
#define VG_BOUNDARY_LAYER_RESISTANCE_H

#include <base/box.h>

namespace vg {

class BoundaryLayerResistance : public base::Box
{
public:
    BoundaryLayerResistance(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double leafDimension, indoorsWindSpeed;

    // Variables
    double rbH2O, rbCO2;

    // Methods
    void updateValue(double windSpeed);
};
} //namespace


#endif
