/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
