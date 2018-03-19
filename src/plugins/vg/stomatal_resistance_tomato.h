/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_STOMATAL_RESISTANCE_TOMATO_H
#define VG_STOMATAL_RESISTANCE_TOMATO_H

#include "stomatal_resistance_base.h"

namespace vg {

class StomatalResistanceTomato : public StomatalResistanceBase
{
public:
    StomatalResistanceTomato(QString name, QObject *parent);

private:
    // Inputs
    double riH2Omin, lai, temperature, radiationAbsorbed,
        rh, co2, indoorsTemperature;
    // Methods
    double resetRsH2O();
    double updateRsH2O();
    double calcRsH2O(double A);
};
} //namespace


#endif
