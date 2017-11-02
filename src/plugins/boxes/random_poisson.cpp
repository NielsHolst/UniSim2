#include <base/publish.h>
#include "random_poisson.h"

using namespace base;

namespace boxes {

PUBLISH(RandomPoisson)

RandomPoisson::RandomPoisson(QString name, QObject *parent)
    : RandomBase<int>(name, parent), distribution(0), variate(0)
{
    help("produces random numbers from the Poisson distribution");
    Input(mean).equals(10);
}

RandomPoisson::~RandomPoisson() {
    delete distribution;
    delete variate;
}

void RandomPoisson::createGenerator() {
    delete distribution;
    delete variate;
    distribution = new Distribution(mean);
    variate = new Variate(*randomGenerator(), *distribution);
}

int RandomPoisson::drawValue() {
    return (*variate)();
}

} //namespace
