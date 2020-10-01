#ifndef AREA_LOSS_RESPIRATION_H
#define AREA_LOSS_RESPIRATION_H
#include <base/box.h>

namespace saccharina {

class AreaLossRespiration : public base::Box
{
public:
    AreaLossRespiration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double A, C, Cmin, Cstruct;
    // Outputs
    double value;
};

}

#endif
