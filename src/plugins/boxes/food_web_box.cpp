/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "food_web_box.h"

using namespace base;

namespace boxes {

PUBLISH(FoodWebBox)

FoodWebBox::FoodWebBox(QString name, QObject *parent)
    : FoodWebBoxBase(name, parent)
{
    help("basic box for FoodWeb models");
}

}
