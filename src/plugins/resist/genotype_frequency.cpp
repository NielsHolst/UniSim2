#include <base/publish.h>
#include "genotype_frequency.h"

using namespace base;

namespace resist {

PUBLISH(GenotypeFrequency)

GenotypeFrequency::GenotypeFrequency(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(N).help("Population size of this genotype");
    Input(Ntotal).help("Total population size");;
    Output(value).help("Frequency of this genotype [0;1]");
}

void GenotypeFrequency::update() {
    value = (Ntotal != 0) ? N/Ntotal : 0;
}

}
