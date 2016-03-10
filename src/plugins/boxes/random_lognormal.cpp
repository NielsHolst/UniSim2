#include <base/publish.h>
#include "random_lognormal.h"

using namespace base;

namespace boxes {

PUBLISH(RandomLognormal)

RandomLognormal::RandomLognormal(QString name, QObject *parent)
    : RandomBase(name, parent), distribution(0), variate(0)
{
    Input(mean).equals(10);
    Input(sd).equals(2);
}

RandomLognormal::~RandomLognormal() {
    delete distribution;
    delete variate;
}

void RandomLognormal::initialize() {
    RandomBase::initialize();
    distribution = new Distribution(mean, sd);
    variate = new Variate(*randomGenerator(), *distribution);
}

double RandomLognormal::drawValue() {
    return (*variate)();
}

} //namespace
