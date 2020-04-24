#ifndef RATE_OF_CHANGE_IN_CARBON_RESERVES_H
#define RATE_OF_CHANGE_IN_CARBON_RESERVES_H
#include <base/box.h>

namespace saccharina {

class Rateofchangeincarbonreserves : public base::Box
{
public:
    Rateofchangeincarbonreserves(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double kA, Gphotosynthesis, Cexudation, Respiration, C, Cstruct, u;
    // Outputs
    double carbonreservesrate;
};

}

#endif
