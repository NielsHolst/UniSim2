#ifndef BPH_SURVIVAL_H
#define BPH_SURVIVAL_H
#include <base/box.h>

namespace bph {

class BphSurvival : public base::Box
{
public:
    BphSurvival(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double density, atDensity, survival;
    // Outputs
    double value;
    // Data
    double _slope;
};

}

#endif
