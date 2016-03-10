/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_layer.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(LeafLayer)

/*! \class LeafLayer
 * \brief A layer of the crop canopy
 *
 * Inputs
 * ------
 * - _xGaussUpperside_ is the LAI coefficient coming from above [0;1]
 * - _wGaussUpperside_ is weighing coefficient coming from above[0;1]
 * - _xGaussLowerside_ is the LAI coefficient coming from below [0;1]
 * - _wGaussLowerside_ is weighing coefficient coming from below[0;1]
 *
 */

LeafLayer::LeafLayer(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(xGaussUpperside).equals(1);
    Input(wGaussUpperside).equals(1);
    Input(xGaussLowerside).equals(1);
    Input(wGaussLowerside).equals(1);
}



} //namespace

