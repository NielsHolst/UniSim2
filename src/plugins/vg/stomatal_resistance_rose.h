/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef STOMATAL_RESISTANCE_ROSE_H
#define STOMATAL_RESISTANCE_ROSE_H

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
