#ifndef EFFECT_OF_TEMPERATURE_ON_GROWTHRATE_H
#define EFFECT_OF_TEMPERATURE_ON_GROWTHRATE_H
#include <base/box.h>

namespace saccharina {

class Effectoftemperatureongrowthrate : public base::Box
{
public:
    Effectoftemperatureongrowthrate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double T;
    // Outputs
    double ftemp;
};

}

#endif
