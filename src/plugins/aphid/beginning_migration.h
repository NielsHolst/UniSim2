#ifndef BEGINNING_MIGRATION_H
#define BEGINNING_MIGRATION_H
#include <base/box.h>

namespace aphid {

class Beginning_migration : public base::Box
{
public:
    Beginning_migration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Temperature;
    double Date;
    // Outputs
    double starting_date; // in Julian days
    // Data
    double AprilDD;
    double MayDD;
};

}

#endif
