#ifndef APH_FECUNDITY_H
#define APH_FECUNDITY_H
#include <base/box.h>

namespace student {

class Aph_fecundity : public base::Box
{
public:
    Aph_fecundity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double proportion_alate;
    double density_apterous;
    double reproduction_rate;
    // Outputs
    double nb_alates_nymph;
    double nb_apterous_nymph;
};

}

#endif
