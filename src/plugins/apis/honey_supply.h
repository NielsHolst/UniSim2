#ifndef HONEY_SUPPLY_H
#define HONEY_SUPPLY_H
#include <base/box.h>

namespace apis {

class HoneySupply : public base::Box
{
public:
    HoneySupply(QString name, QObject *parent);
    void update();
private:
    // input
    QVector<double> cohortNumbers;
    double cropVolume, handlingTime, conversionFactor,
        nectarFlow, timeStepSecs;
    // output
    double value, rate;
};

}

#endif
