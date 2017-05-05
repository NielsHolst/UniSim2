#ifndef SURVIVAL_BY_DENSITY_H
#define SURVIVAL_BY_DENSITY_H
#include <base/box.h>

namespace cmbp {

class SurvivalByDensity : public base::Box
{
public:
    SurvivalByDensity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double e1, N;
    // Output
    double value;
};

}

#endif
