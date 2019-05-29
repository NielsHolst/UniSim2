#include <base/publish.h>
#include "sobol_sequence_binomial.h"

using namespace base;

namespace boxes {

PUBLISH(SobolSequenceBinomial)

SobolSequenceBinomial::SobolSequenceBinomial(QString name, QObject *parent)
    : RandomBaseTyped<bool>(name, parent), distribution(nullptr), variate(nullptr)
{
    help("produces a quasi-random Sobol' sequence from the binary distribution");
}

SobolSequenceBinomial::~SobolSequenceBinomial() {
    delete distribution;
    delete variate;
}

void SobolSequenceBinomial::initalizeGenerator() {
    delete distribution;
    delete variate;
    distribution = new Distribution;
    variate = new Variate(*Randomiser::sobolGenerator(), *distribution);
}

void SobolSequenceBinomial::updateValue(int) {
    double u = (*variate)();
    value = (u<P);
}

} //namespace
