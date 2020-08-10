/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/random_order.h>
#include "random_log_uniform.h"
#include "randomiser_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomLogUniform)

RandomLogUniform::RandomLogUniform(QString name, QObject *parent)
    : RandomBaseTyped<double>(name, parent)
{
    help("produces random numbers from the log uniform distribution");
    port("P")->equals(0).help("Not used for this distribution");
}

void RandomLogUniform::updateValue() {
    if (useFixed) {
        value = fixed;
    }
    else {
        int stratum = _order->next(),
            numStrata = _order->size();
        double u = randomiser()->draw01(),
               w = (log(max)-log(min))/numStrata,
               log_value = log(min) + (stratum + u)*w;
        value = exp(log_value);
    }
}

} //namespace
