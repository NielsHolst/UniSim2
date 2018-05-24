#ifndef R0_SCHMITZ_H
#define R0_SCHMITZ_H
#include <base/box.h>

namespace aphid {

class R0_Schmitz : public base::Box
{
public:
    R0_Schmitz(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Tmin;
    double Tmax;
    double Topt;
    double R0_opt;
    double Temperature;
    double Wheat_Pheno;
    // Outputs
    double lifetime_fec_apterous;
    double lifetime_fec_alates;
};

}

#endif
