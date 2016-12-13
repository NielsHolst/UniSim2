#ifndef DENSITY_PER_TILLER_H
#define DENSITY_PER_TILLER_H
#include <base/box.h>

namespace student {

class Density_per_tiller : public base::Box
{
public:
    Density_per_tiller(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double nb_nymph_apterous;
    double nb_nymph_alates;
    double nb_adult_apterous;
    double nb_adult_alates;
    double nb_plant;
    // Outputs
    int density;
};

}

#endif
