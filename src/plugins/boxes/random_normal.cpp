#include <base/phys_math.h>
#include <base/publish.h>
#include <base/random_order.h>
#include "random_normal.h"
#include "randomiser_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomNormal)

RandomNormal::RandomNormal(QString name, QObject *parent)
    : RandomBaseTyped<double>(name, parent)
{
    help("produces random numbers from the normal distribution");
}


void RandomNormal::updateValue() {
    if (useFixed) {
        value = fixed;
    }
    else {
        int stratum = _order->next(),
            numStrata = _order->size();
        double u = randomiser()->draw01(),
               w = 1./numStrata,
               u2 = (stratum + u)*w;
        value = phys_math::invNormalRange(u2, min, max, P);
    }
}

} //namespace
