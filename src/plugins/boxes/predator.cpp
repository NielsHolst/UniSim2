/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/publish.h>
#include "predator.h"
using namespace base;

namespace boxes {

PUBLISH(Predator)

Predator::Predator(QString name, QObject *parent)
    : Box(name, parent) {
    help("holds the demand and supply of a predator");
    Input(density).help("Density");
    Input(demand).help("Amount of resource needed");
    Output(supply).help("Total amount of resource acquired");
    Output(sdRatio).help("Total supply/demand ratio").unit("[0;1]");
}

void Predator::amend() {
    setFoodWebImports();
}

void Predator::setFoodWebImports() {
    Box *web = findOne<Box>("ancestors::*<FoodWeb>");
    QString prefix = web->fullName(),
            suffix = "_" + name();
    port("supply")->imports(prefix + "[supply" + suffix + "]");
    port("sdRatio")->imports(prefix + "[sdRatio" + suffix + "]");
}

} //namespace

