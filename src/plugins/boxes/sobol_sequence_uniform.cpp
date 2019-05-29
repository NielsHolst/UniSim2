#include <base/publish.h>
#include "sobol_sequence_uniform.h"

#include <base/dialog.h>

using namespace base;

namespace boxes {

PUBLISH(SobolSequenceUniform)

SobolSequenceUniform::SobolSequenceUniform(QString name, QObject *parent)
    : RandomBaseTyped<double>(name, parent), distribution(nullptr), variate(nullptr)
{
    help("produces a quasi-random Sobol' sequence from the uniform distribution");
}

SobolSequenceUniform::~SobolSequenceUniform() {
    delete distribution;
    delete variate;
}

void SobolSequenceUniform::initalizeGenerator() {
    delete distribution;
    delete variate;
    distribution = new Distribution;
    variate = new Variate(*Randomiser::sobolGenerator(), *distribution);
}

void SobolSequenceUniform::updateValue(int) {
    dialog().information("SobolSequenceUniform::updateValue A");
    double u = (*variate)(),
           w = max-min;
    value = min + u*w;
    dialog().information("SobolSequenceUniform::updateValue Z");
}

} //namespace
