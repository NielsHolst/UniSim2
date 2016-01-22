#include <boxes/publish.h>
#include "random_lognormal.h"

using namespace boxes;

namespace base {

PUBLISH(RandomLognormal)

RandomLognormal::RandomLognormal(QString name, QObject *parent)
    : RandomBase(name, parent), distribution(0), variate(0)
{
    Class(RandomLognormal);
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