#include <base/phys_math.h>
#include <base/publish.h>
#include "sobol_sequence_lognormal.h"

using namespace base;

namespace boxes {

PUBLISH(SobolSequenceLognormal)

SobolSequenceLognormal::SobolSequenceLognormal(QString name, QObject *parent)
    : RandomBaseTyped<double>(name, parent), distribution(nullptr), variate(nullptr)
{
    help("produces a quasi-random Sobol' sequence from the lognormal distribution");
}

SobolSequenceLognormal::~SobolSequenceLognormal() {
    delete distribution;
    delete variate;
}

void SobolSequenceLognormal::initalizeGenerator() {
    delete distribution;
    delete variate;
    distribution = new Distribution;
    variate = new Variate(*Randomiser::sobolGenerator(), *distribution);
}

void SobolSequenceLognormal::updateValue(int) {
    if (min<=0.)
        ThrowException("Minimum value must be positive").value(min).context(this);
    if (max<=0.)
        ThrowException("Maximum value must be positive").value(max).context(this);
    double u = (*variate)();
    value = exp( phys_math::invNormalRange(u, log(min), log(max), P) );
}

} //namespace
