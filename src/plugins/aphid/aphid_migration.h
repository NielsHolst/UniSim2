#ifndef APHID_MIGRATION_H
#define APHID_MIGRATION_H
#include <base/box.h>

namespace aphid {

class Aphid_migration : public base::Box
{
public:
    Aphid_migration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Wheat_Pheno;
    double Date;
    double Daily_Nb_migrants;
    // Outputs
    double migration;
};

}

#endif
