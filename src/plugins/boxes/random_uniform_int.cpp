#include <base/publish.h>
#include "random_uniform_int.h"

using namespace base;

namespace boxes {

PUBLISH(RandomUniformInt)

RandomUniformInt::RandomUniformInt(QString name, QObject *parent)
    : RandomBase<int>(name, parent), distribution(0), variate(0)
{
    // Replace default bounds
    port("minValue")->equals(0);
    port("maxValue")->equals(1);
}

RandomUniformInt::~RandomUniformInt() {
    delete distribution;
    delete variate;
}

void RandomUniformInt::doInitialize() {
    distribution = new Distribution(minValue, maxValue);
    variate = new Variate(*randomGenerator(), *distribution);
}

int RandomUniformInt::drawValue() {
    return (*variate)();
}

} //namespace
