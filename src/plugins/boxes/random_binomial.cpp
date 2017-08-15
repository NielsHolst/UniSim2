#include <base/publish.h>
#include "random_binomial.h"

using namespace base;

namespace boxes {

PUBLISH(RandomBinomial)

RandomBinomial::RandomBinomial(QString name, QObject *parent)
    : RandomBase<bool>(name, parent), distribution(0), variate(0)
{
    help("produces random numbers from the binomial distribution");
    Input(P).equals(0.5).help("Probability of event");
}

RandomBinomial::~RandomBinomial() {
    delete distribution;
    delete variate;
}

void RandomBinomial::doInitialize() {
    distribution = new Distribution(0, 1);
    variate = new Variate(*randomGenerator(), *distribution);
}

bool RandomBinomial::drawValue() {
    return (*variate)() > P;
}

void RandomBinomial::nextValue() {
    value = drawValue();
}

} //namespace
