#ifndef APH_FECUNDITY_H
#define APH_FECUNDITY_H
#include <base/box.h>

namespace aphid {

class Aph_fecundity : public base::Box
{
public:
    Aph_fecundity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double proportion_alate;
    double number_immigrants;
    double number_apterous;
    double reproduction_rate;
    // Outputs
    double nb_alates_nymph;
    double nb_apterous_nymph;
    // Date
    double nb_adult_total;
};

}

#endif
