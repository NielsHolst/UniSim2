#ifndef FECUNDITY_H
#define FECUNDITY_H
#include <base/box.h>

namespace apis {

class Fecundity : public base::Box
{
public:
    Fecundity(QString name, QObject *parent);
    void update();
private:
    // input
    double slope, intercept, brood, workers, maxFecundity, timeStepDays;
    // output
    double value, rate;
};

}

#endif
