/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FOOD_WEB_BOX_H
#define FOOD_WEB_BOX_H
#include <base/food_web_box_base.h>

namespace boxes {

class FoodWebBox : public base::FoodWebBoxBase
{
public:
    FoodWebBox(QString name, QObject *parent);
};

}

#endif
