#ifndef APHID_MIGRATION2_H
#define APHID_MIGRATION2_H
#include <base/box.h>

namespace aphid {

class Aphid_migration2 : public base::Box
{
public:
    Aphid_migration2(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Wheat_Pheno;
    double Date;
    double Daily_Nb_migrants;
    double perc_infest;
    // Outputs
    double migration;
    double migration_infect;
};

}

#endif
