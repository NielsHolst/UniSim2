#ifndef PARAM_GAL_H
#define PARAM_GAL_H
#include <base/box.h>

namespace aphid {

class Param_gal : public base::Box
{
public:
    Param_gal(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double k;
    double kReproHealthy;
};
}

#endif
