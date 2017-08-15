#include <base/publish.h>
#include "random_uniform.h"

using namespace base;

namespace boxes {

PUBLISH(RandomUniform)

RandomUniform::RandomUniform(QString name, QObject *parent)
    : RandomBase<double>(name, parent), distribution(0), variate(0)
{
    help("produces random numbers from the uniform distribution");
    // Replace default bounds
    port("minValue")->equals(0);
    port("maxValue")->equals(1);
}

RandomUniform::~RandomUniform() {
    delete distribution;
    delete variate;
}

void RandomUniform::doInitialize() {
    distribution = new Distribution(minValue, maxValue);
    variate = new Variate(*randomGenerator(), *distribution);
}

double RandomUniform::drawValue() {
    return (*variate)();
}

} //namespace
