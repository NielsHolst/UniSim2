#include <base/publish.h>
#include "random_uniform.h"

using namespace base;

namespace boxes {

PUBLISH(RandomUniform)

RandomUniform::RandomUniform(QString name, QObject *parent)
    : RandomBase(name, parent), distribution(0), variate(0)
{
}

RandomUniform::~RandomUniform() {
    delete distribution;
    delete variate;
}

void RandomUniform::initialize() {
    RandomBase::initialize();
    distribution = new Distribution(port("minValue")->value<double>(),
                                    port("maxValue")->value<double>());
    variate = new Variate(*randomGenerator(), *distribution);
}

double RandomUniform::drawValue() {
    return (*variate)();
}

} //namespace
