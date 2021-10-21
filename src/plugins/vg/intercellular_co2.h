/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef INTERCELLULAR_CO2_H
#define INTERCELLULAR_CO2_H

#include <base/box.h>

namespace vg {

class IntercellularCo2 : public base::Box
{
public:
    IntercellularCo2(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        boundaryLayerResistance, stomatalResistance, indoorsCo2,
        leafPhotosynthesis;
    // Outputs
    double value;
};
} //namespace


#endif