/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_STOMATAL_RESISTANCE_BASE_H
#define VG_STOMATAL_RESISTANCE_BASE_H

#include <base/box.h>

namespace vg {

class StomatalResistanceBase : public base::Box
{
public:
    StomatalResistanceBase(QString name, QObject *parent);
    void reset() final;
    void update() final;

private:
    // Outputs
    double rsH2O, rsCo2;
    // Methods
    virtual double resetRsH2O() = 0;
    virtual double updateRsH2O() = 0;
};
} //namespace


#endif
