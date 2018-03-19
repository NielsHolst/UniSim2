/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_STOMATAL_RESISTANCE_ROSE_H
#define VG_STOMATAL_RESISTANCE_ROSE_H

#include "stomatal_resistance_base.h"

namespace vg {

class StomatalResistanceRose : public StomatalResistanceBase
{
public:
    StomatalResistanceRose(QString name, QObject *parent);

private:
    // Inputs
    double rh, rbCO2, co2, Pn, lai;
    // Methods
    double resetRsH2O();
    double updateRsH2O();
    double calcRsH2O(double A);
};
} //namespace


#endif
