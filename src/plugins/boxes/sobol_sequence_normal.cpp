#include <base/phys_math.h>
#include <base/publish.h>
#include "sobol_sequence_normal.h"

#include <base/dialog.h>

using namespace base;

namespace boxes {

PUBLISH(SobolSequenceNormal)

SobolSequenceNormal::SobolSequenceNormal(QString name, QObject *parent)
    : RandomBaseTyped<double>(name, parent), distribution(nullptr), variate(nullptr)
{
    help("produces a quasi-random Sobol' sequence from the uniform distribution");
}

SobolSequenceNormal::~SobolSequenceNormal() {
    delete distribution;
    delete variate;
}

void SobolSequenceNormal::initalizeGenerator() {
    delete distribution;
    delete variate;
    distribution = new Distribution;
    variate = new Variate(*Randomiser::sobolGenerator(), *distribution);
}

void SobolSequenceNormal::updateValue(int) {
    double u = (*variate)();
    value = phys_math::invNormalRange(u, min, max, P);
}

} //namespace
