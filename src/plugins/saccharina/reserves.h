#ifndef RESERVES_H
#define RESERVES_H
#include <base/box.h>

namespace saccharina {

class Reserves : public base::Box
{
public:
    Reserves(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initialProportion, maxProportion, allocatedProportion;
    // Outputs
    double proportion;
};

}

#endif
