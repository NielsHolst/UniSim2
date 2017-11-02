#include <base/publish.h>
#include <base/vector_op.h>
#include "check_vector.h"
#include "allele_frequency.h"

using namespace base;

namespace resist {

PUBLISH(AlleleFrequency)

AlleleFrequency::AlleleFrequency(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes allele frequencies from genotypes");
    Input(genotypeFrequency).help("Genotype frequency (ss,sr,rr");
    Output(p).help("Frequency of S allele");
    Output(q).help("Frequency of R allele");
}

void AlleleFrequency::reset() {
    update();
}

void AlleleFrequency::update() {
    double sum = 2*vector_op::sum(genotypeFrequency);
    if (sum == 0.)
        p = q = 0;
    else {
        p = (2*genotypeFrequency.at(0) + genotypeFrequency.at(1))/sum;
        q = 1. - p;
    }
}

}
