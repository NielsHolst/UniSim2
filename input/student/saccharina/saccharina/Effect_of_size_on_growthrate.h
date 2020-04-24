#ifndef EFFECT_OF_SIZE_ON_GROWTHRATE_H
#define EFFECT_OF_SIZE_ON_GROWTHRATE_H
#include <base/box.h>

namespace saccharina {

class Effectofsizeongrowthrate : public base::Box
{
public:
    Effectofsizeongrowthrate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double m1, A, m2, A0;
    // Outputs
    double farea;
};

}

#endif
