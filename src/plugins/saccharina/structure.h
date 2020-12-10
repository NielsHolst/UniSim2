#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <base/box.h>

namespace saccharina {

class Structure : public base::Box
{
public:
    Structure(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double proportionC, proportionN,
        initMass, allocation;
    // Outputs
    double mass;
};

}

#endif
