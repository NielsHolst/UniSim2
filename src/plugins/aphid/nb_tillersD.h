#ifndef NB_TILLERSD_H
#define NB_TILLERSD_H
#include <base/box.h>

namespace aphid {

class Nb_tillersD : public base::Box
{
public:
    Nb_tillersD(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Wheat_Pheno;
    // Outputs
    int nb_tillers;
};

}

#endif
