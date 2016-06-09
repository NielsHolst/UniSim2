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
    double minFlow, maxFlow, concentration, timeStepSecs;
    QDate beginDate, endDate, currentDate;
    QTime beginTime, endTime, currentTime;
    // output
    double flow;
};

}

#endif
