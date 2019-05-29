/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef RANDOM_NORMAL_H
#define RANDOM_NORMAL_H
#include "random_base_typed.h"

namespace boxes {

class RandomNormal : public RandomBaseTyped<double>
{
public: 
    RandomNormal(QString name, QObject *parent);
private:
    void updateValue();
};

} //namespace
#endif
