#ifndef SURVIVAL_H
#define SURVIVAL_H
#include <base/box.h>

namespace resist {

class Survival : public base::Box
{
public:
    Survival(QString name, QObject *parent);
private:
    // Inputs
    double value;
};

}

#endif
