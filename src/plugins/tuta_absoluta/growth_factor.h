#ifndef GROWTH_FACTOR_H
#define GROWTH_FACTOR_H
#include <base/box.h>

namespace tuta_absoluta {

class GrowthFactor : public base::Box
{
public:
    GrowthFactor(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double sexRatio, eggsPerFemale;
    // Outputs
    double growth_Factor;
};

}

#endif
