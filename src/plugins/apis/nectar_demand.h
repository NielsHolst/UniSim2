#ifndef NECTAR_DEMAND_H
#define NECTAR_DEMAND_H
#include <QDate>
#include <QTime>
#include <base/box.h>

namespace apis {

class NectarDemand : public base::Box
{
public:
    NectarDemand(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    QVector<double> numberWorkers, numberLarvae, massIncrement;
    double timeStepDays, conversionRatio, honeyConcentration, nectarConcentration, workerDemand;
    // output
    double demandLarvae, demandWorkers, demandTotal;
};

}

#endif
