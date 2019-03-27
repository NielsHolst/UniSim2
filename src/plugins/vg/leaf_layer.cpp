/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "leaf_layer.h"
#include <base/exception.h>
#include <base/publish.h>
#include "general.h"
using namespace base;

namespace vg {

PUBLISH(LeafLayer)

LeafLayer::LeafLayer(QString name, QObject *parent)
	: Box(name, parent)
{
    help("holds parameters for Gaussian integration over leaf canopy");
    Input(xGaussUpperside).help("LAI coefficient coming from above").unit("[0;1]");
    Input(wGaussUpperside).help("weighing coefficient coming from above").unit("[0;1]");
    Input(xGaussLowerside).help("LAI coefficient coming from below").unit("[0;1]");
    Input(wGaussLowerside).help("weighing coefficient coming from below").unit("[0;1]");
}

void LeafLayer::initialize() {
    if (objectName() == "top") {
        xGaussUpperside = xGauss3[0];
        wGaussUpperside = wGauss3[0];
        xGaussLowerside = xGauss3[2];
        wGaussLowerside = wGauss3[2];
    }
    else if (objectName() == "middle") {
        xGaussUpperside = xGauss3[1];
        wGaussUpperside = wGauss3[1];
        xGaussLowerside = xGauss3[1];
        wGaussLowerside = wGauss3[1];
    }
    else if (objectName() == "bottom") {
        xGaussUpperside = xGauss3[2];
        wGaussUpperside = wGauss3[2];
        xGaussLowerside = xGauss3[0];
        wGaussLowerside = wGauss3[0];
    }
    else {
        ThrowException("Wrong layer name").value(objectName()).context(this);
    }
}


} //namespace

