#ifndef POTENTIAL_FECUNDITY_DUFFY_H
#define POTENTIAL_FECUNDITY_DUFFY_H
#include <base/box.h>

namespace aphid {

class Potential_fecundity_Duffy : public base::Box
{
public:
    Potential_fecundity_Duffy(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Temperature;
    double Wheat_Pheno;
    // Outputs
    double alate_potential;
    double apterous_potential;

};

}

#endif
