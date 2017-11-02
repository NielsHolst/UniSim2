#ifndef ALLELE_FREQUENCY_H
#define ALLELE_FREQUENCY_H
#include <base/box.h>

namespace resist {

class AlleleFrequency : public base::Box
{
public:
    AlleleFrequency(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double> genotypeFrequency;
    // Outputs
    double p, q;
};

}

#endif
