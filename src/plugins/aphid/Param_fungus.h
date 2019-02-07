#ifndef PARAM_FUNGUS_H
#define PARAM_FUNGUS_H
#include <base/box.h>

namespace aphid {

class Param_fungus : public base::Box
{
public:
    Param_fungus(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double T0Fungus;
    double ToptFungus;
    double TmaxFungus;
    double Threshold_RH;
    double virulence;
};

}

#endif
