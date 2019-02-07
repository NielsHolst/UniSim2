#ifndef APHID_MIGRATION_LIMITED_H
#define APHID_MIGRATION_LIMITED_H
#include <base/box.h>

namespace aphid {

class Aphid_migration_limited : public base::Box
{
public:
    Aphid_migration_limited(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Wheat_Pheno;
    double Beginning;
    double End;
    double Date;
    double Daily_Nb_migrants;
    double AphidDensity;
    double DensityMax;
    // Outputs
    double migration;
};

}

#endif
