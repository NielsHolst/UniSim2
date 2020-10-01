#ifndef AREA_H
#define AREA_H
#include <base/box.h>

namespace saccharina {

class Area : public base::Box
{
public:
    Area(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initialValue, growthRate, lossRateErosion, lossRespiration, timeStepDays;
    QDate date;
    // Outputs
    double value, logValue, pctGrowth;
    // Data
    double prevValue;
    QDate prevDate;
};

}

#endif
