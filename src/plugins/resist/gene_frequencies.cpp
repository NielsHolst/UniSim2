#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "gene_frequencies.h"

using namespace base;

namespace resist {

PUBLISH(GeneFrequencies)

GeneFrequencies::GeneFrequencies(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(N).help("Population size vector (ss, sr, rr)");
    Output(ss).help("Frequency ss genotype [0;1]");
    Output(sr).help("Frequency ss genotype [0;1]");
    Output(rr).help("Frequency ss genotype [0;1]");
    Output(s).help("Frequency s allele [0;1]");
    Output(r).help("Frequency r allele [0;1]");
}

void GeneFrequencies::update() {
    // Compute genotype proportions
    double sum = vector_op::sum(N);
    if (sum == 0)
        ss = sr = rr = r = s = 0.;
    else {
        ss = N[0]/sum;
        sr = N[1]/sum;
        rr = N[2]/sum;
        s = ss + sr/2;
        r = rr + sr/2;
    }
}

}