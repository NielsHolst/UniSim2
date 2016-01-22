#include <base/publish.h>
#include "random_poisson.h"

using namespace base;

namespace boxes {

PUBLISH(RandomPoisson)

RandomPoisson::RandomPoisson(QString name, QObject *parent)
    : RandomBase(name, parent), distribution(0), variate(0)
{
    Input(mean).equals(10);
}

RandomPoisson::~RandomPoisson() {
    delete distribution;
    delete variate;
}

void RandomPoisson::initialize() {
    RandomBase::initialize();
    distribution = new Distribution(mean);
    variate = new Variate(*randomGenerator(), *distribution);
}

double RandomPoisson::drawValue() {
    return (*variate)();
}

} //namespace
