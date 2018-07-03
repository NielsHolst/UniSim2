/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VENT_OPENING_H
#define VENT_OPENING_H

#include <base/box.h>

namespace vg {

class VentOpening : public base::Box
{
public:
    VentOpening(QString name, QObject *parent);
    void update();
private:
    // Inputs;
    double airFluxSupply, airFluxSupplyMax, ventOpeningExponent;
    // Outputs
    double value;
};
} //namespace


#endif
