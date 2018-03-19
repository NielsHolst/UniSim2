/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_LEAF_LAYER_H
#define VG_LEAF_LAYER_H

#include <base/box.h>

namespace vg {

class LeafLayer : public base::Box
{
public:
    LeafLayer(QString name, QObject *parent);

private:
    double xGaussUpperside, wGaussUpperside,
        xGaussLowerside, wGaussLowerside;
};
} //namespace


#endif
