/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/random_order.h>
#include "random_log_normal.h"
#include "randomiser_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomLogNormal)

RandomLogNormal::RandomLogNormal(QString name, QObject *parent)
    : RandomBaseTyped<double>(name, parent)
{
    help("produces random numbers from the log-normal distribution");
}

void RandomLogNormal::updateValue() {
    if (useFixed) {
        value = fixed;
    }
    else {
        int stratum = _order->next(),
            numStrata = _order->size();
        if (min<=0.)
            ThrowException("Minimum value must be positive").value(min).context(this);
        if (max<=0.)
            ThrowException("Maximum value must be positive").value(max).context(this);
        double u = randomiser()->draw01(),
               w = 1./numStrata,
               u2 = (stratum + u)*w;
        value = exp( phys_math::invNormalRange(u2, log(min), log(max), P) );
    }
}

} //namespace