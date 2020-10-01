#ifndef AREA_LOSS_EROSION_H
#define AREA_LOSS_EROSION_H
#include <base/box.h>

namespace saccharina {

class AreaLossErosion : public base::Box
{
public:
    AreaLossErosion(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double A, eps;
    // Outputs
    double value;
};

}

#endif
