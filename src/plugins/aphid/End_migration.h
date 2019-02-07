#ifndef END_MIGRATION_H
#define END_MIGRATION_H
#include <base/box.h>

namespace aphid {

class End_migration : public base::Box
{
public:
    End_migration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double GS;
    double GS_max;
    double Date;
    // Outputs
    double stopping_date; // in Julian days

};

}

#endif
