#include <boxes/publish.h>
#include "random_normal.h"

using namespace boxes;

namespace base {

PUBLISH(RandomNormal)

RandomNormal::RandomNormal(QString name, QObject *parent)
    : RandomBase(name, parent), distribution(0), variate(0)
{
    Class(RandomNormal);
    Input(mean).equals(10);
    Input(sd).equals(2);
}

RandomNormal::~RandomNormal() {
    delete distribution;
    delete variate;
}

void RandomNormal::initialize() {
    RandomBase::initialize();
    distribution = new Distribution(mean, sd);
    variate = new Variate(*randomGenerator(), *distribution);
}

double RandomNormal::drawValue() {
    return (*variate)();
}

} //namespace
