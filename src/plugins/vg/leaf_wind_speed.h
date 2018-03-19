/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_LEAF_WIND_SPEED_H
#define VG_LEAF_WIND_SPEED_H

#include <base/box.h>

namespace vg {

class LeafWindSpeed : public base::Box
{
public:
    LeafWindSpeed(QString name, QObject *parent);
    void reset();
    void update();

protected:
    // Inputs
    double k ,indoorsWindSpeedMinimum, indoorsWindSpeed,
        xGauss, lai;
    // Outputs
    double value;
};

} //namespace

#endif
