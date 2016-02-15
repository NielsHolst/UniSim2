/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BOUNDARY_LAYER_RESISTANCE_BASE_H
#define BOUNDARY_LAYER_RESISTANCE_BASE_H

#include <base/box.h>

namespace vg {

class BoundaryLayerResistanceBase : public base::Box
{
public:
    BoundaryLayerResistanceBase(QString name, QObject *parent);
    void reset();
    void update() final;
protected:
    // Inputs
    double leafDimension, leafWindSpeed;
    // Outputs
    double rbH2O, rbCO2;
private:
    // Methods
    virtual void setRbH2O() = 0;
};

} //namespace

#endif
