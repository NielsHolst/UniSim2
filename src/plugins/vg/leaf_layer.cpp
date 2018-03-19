/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "leaf_layer.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(LeafLayer)

LeafLayer::LeafLayer(QString name, QObject *parent)
	: Box(name, parent)
{
    help("holds parameters for Gaussian integration over leaf canopy");
    Input(xGaussUpperside).help("LAI coefficient coming from above [0;1]");
    Input(wGaussUpperside).help("weighing coefficient coming from above [0;1]");
    Input(xGaussLowerside).help("LAI coefficient coming from below [0;1]");
    Input(wGaussLowerside).help("weighing coefficient coming from below [0;1]");
}



} //namespace

