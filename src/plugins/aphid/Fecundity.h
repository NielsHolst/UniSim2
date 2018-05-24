#ifndef FECUNDITY_H
#define FECUNDITY_H
#include <base/box.h>

namespace aphid {

class Fecundity : public base::Box
{
public:
    Fecundity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double potential_fec_alates; // fec. per adults (differs depending on morph)
    double potential_fec_apterous;
    double number_immigrants;
    double number_apterous;
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
