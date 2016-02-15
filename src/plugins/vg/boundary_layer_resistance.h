/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
