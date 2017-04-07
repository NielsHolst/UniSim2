#ifndef MEDIATOR_H
#define MEDIATOR_H
#include <base/box.h>

namespace student {

class Mediator : public base::Box
{
public:
    Mediator(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        preference, B0, h,
    // From plant
        R,
    // From pollinator
        x, y, B;
    // Outputs
    double cr;
    // Data
    int previous;
    // Methods
    double compute_F();
};

}

#endif
