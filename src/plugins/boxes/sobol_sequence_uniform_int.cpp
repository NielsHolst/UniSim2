#include <base/publish.h>
#include "sobol_sequence_uniform_int.h"

using namespace base;

namespace boxes {

PUBLISH(SobolSequenceUniformInt)

SobolSequenceUniformInt::SobolSequenceUniformInt(QString name, QObject *parent)
    : RandomBaseTyped<int>(name, parent), distribution(nullptr), variate(nullptr)
{
    help("produces a quasi-random Sobol' sequence of integers from the uniform distribution");
}

SobolSequenceUniformInt::~SobolSequenceUniformInt() {
    delete distribution;
    delete variate;
}

void SobolSequenceUniformInt::initalizeGenerator() {
    delete distribution;
    delete variate;
    distribution = new Distribution;
    variate = new Variate(*Randomiser::sobolGenerator(), *distribution);
}

void SobolSequenceUniformInt::updateValue(int) {
    double u = (*variate)(),
           w = double(max-min+1);
    value = static_cast<int>( min + std::floor(u*w) );
}

} //namespace
