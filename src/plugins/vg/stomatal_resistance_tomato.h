/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef STOMATAL_RESISTANCE_TOMATO_H
#define STOMATAL_RESISTANCE_TOMATO_H

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
