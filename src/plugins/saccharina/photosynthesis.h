#ifndef PHOTOSYNTHESIS_H
#define PHOTOSYNTHESIS_H
#include <base/box.h>

namespace saccharina {

class Photosynthesis : public base::Box
{
public:
    Photosynthesis(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double crownZoneArea, lai, frondAngle, plantDensity,
        demand, alpha, fTemp, fNitrogen, I;
    // Outputs
    double supply, sdRatio, Iabsorbed;
};

}

#endif
