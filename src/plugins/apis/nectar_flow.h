#ifndef NECTAR_FLOW_H
#define NECTAR_FLOW_H
#include <QDate>
#include <QTime>
#include <base/box.h>

namespace apis {

class NectarFlow : public base::Box
{
public:
    NectarFlow(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    double minFlow, maxFlow;
    QDate beginDate, endDate;
    QTime beginTime, endTime;
    QDateTime currentDateTime;
    double timeStepSecs;
    // output
    double value, rate;
    // methods
    void updateValue();
};

}

#endif
