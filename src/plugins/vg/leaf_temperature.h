/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_LEAF_TEMPERATURE_H
#define VG_LEAF_TEMPERATURE_H

#include <base/box.h>

namespace vg {

class LeafTemperature : public base::Box
{
public:
    LeafTemperature(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double indoorsTemperature, indoorsRh, rsH2O, rbH2O,
        radiationAbsorbed;
    // Outputs
    double value;
};
} //namespace


#endif
