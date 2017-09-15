#ifndef CARRYING_CAPACITY_H
#define CARRYING_CAPACITY_H
#include <base/box.h>

namespace resist {

class CarryingCapacity : public base::Box
{
public:
    CarryingCapacity(QString name, QObject *parent);
private:
    // Inputs
    double value;
};

}

#endif
