#include <base/publish.h>
#include "random_normal.h"

using namespace base;

namespace boxes {

PUBLISH(RandomNormal)

RandomNormal::RandomNormal(QString name, QObject *parent)
    : RandomBase<double>(name, parent), distribution(0), variate(0)
{
    help("produces random numbers from the normal distribution");
    Input(mean).equals(10);
    Input(sd).equals(2);
}

RandomNormal::~RandomNormal() {
    delete distribution;
    delete variate;
}

void RandomNormal::createGenerator() {
    delete distribution;
    delete variate;
    distribution = new Distribution(mean, sd);
    variate = new Variate(*randomGenerator(), *distribution);
}

double RandomNormal::drawValue() {
    return (*variate)();
}

} //namespace
