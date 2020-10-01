#ifndef BIOMASS_H
#define BIOMASS_H
#include <base/box.h>

namespace saccharina {

class Biomass : public base::Box
{
public:
    Biomass(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double kA, Cstruct, Nstruct, kdw, A, C, N;
    // Outputs
    double structure,
        carbonReserve, nitrogenReserve,
        carbonTotal, nitrogenTotal, CNratio, nitrogenPct,
        total, totalWetWeight;
};

}

#endif
