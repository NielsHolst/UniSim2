#ifndef APHID_IMMIGRATION_H
#define APHID_IMMIGRATION_H
#include <base/box.h>

namespace aphid {

class AphidImmigration : public base::Box
{
public:
    AphidImmigration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double cropGrowthStage, immigrationRate, propExposedImmigrants;
    int k;
    // Outputs
    double total, susceptible;
    QVector<double> exposed;
};

}

#endif
