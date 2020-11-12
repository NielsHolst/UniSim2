#ifndef AREA_H
#define AREA_H
#include <base/box.h>

namespace saccharina {

class Area : public base::Box
{
public:
    Area(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double structuralMass, kA, plantDensity;
    // Outputs
    double value, lai;
};

}

#endif
