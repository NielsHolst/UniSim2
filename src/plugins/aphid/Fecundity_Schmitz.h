#ifndef FECUNDITY_SCHMITZ_H
#define FECUNDITY_SCHMITZ_H
#include <base/box.h>

namespace aphid {

class Fecundity_Schmitz : public base::Box
{
public:
    Fecundity_Schmitz(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double potential_fec_alates; // fec. total for all adults ? (differs depending on morph)
    double potential_fec_apterous;
    double aphid_density;
    double Wheat_Pheno;
    // Outputs
    double nb_alates_nymph;
    double nb_apterous_nymph;
    double perc_alates;
    // Date
    double nb_nymph_total;

};

}

#endif
