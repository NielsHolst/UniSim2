#ifndef GENOTYPE_FREQUENCY_H
#define GENOTYPE_FREQUENCY_H
#include <base/box.h>

namespace resist {

class GenotypeFrequency : public base::Box
{
public:
    GenotypeFrequency(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double N, Ntotal;
    // Outputs
    double value;
};

}

#endif
