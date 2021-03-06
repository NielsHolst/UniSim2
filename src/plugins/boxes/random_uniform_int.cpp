/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <cmath>
#include <base/publish.h>
#include <base/random_order.h>
#include "random_uniform_int.h"
#include "randomiser_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomUniformInt)

RandomUniformInt::RandomUniformInt(QString name, QObject *parent)
    : RandomBaseTyped<int>(name, parent)
{
    help("produces random integer numbers from the uniform distribution");
    port("P")->equals(0).help("Not used for this distribution");
    port("max")->help("Maximum value (included)");
}

void RandomUniformInt::updateValue() {
    if (useFixed) {
        value = fixed;
    }
    else {
        int stratum = _order->next(),
            numStrata = _order->size();
        double u = randomiser()->draw01(),
               w = double(max-min+1)/numStrata;
        value = static_cast<int>( min + std::floor((stratum + u)*w) );
    }
}

} //namespace
