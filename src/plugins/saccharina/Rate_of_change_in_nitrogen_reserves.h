#ifndef RATE_OF_CHANGE_IN_NITROGEN_RESERVES_H
#define RATE_OF_CHANGE_IN_NITROGEN_RESERVES_H
#include <base/box.h>

namespace saccharina {

class Rateofchangeinnitrogenreserves : public base::Box
{
public:
    Rateofchangeinnitrogenreserves(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double kA;
    double J;
    double u;
    double N;
    double Nstruct;
    double Factor;
    // Outputs
    double nitrogenreservesrate;
};

}

#endif
