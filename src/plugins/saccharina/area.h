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
    double structuralMass, plantDensity, frondAngle, l, a, b;
    // Outputs
    double value, length, lai, crownZoneArea, kA;
};

}

#endif
