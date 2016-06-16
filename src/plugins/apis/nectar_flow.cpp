#include <base/any_year.h>
#include <base/publish.h>
#include "nectar_flow.h"

using namespace base;

namespace apis {

PUBLISH(NectarFlow)

NectarFlow::NectarFlow(QString name, QObject *parent)
    : Box(name, parent)
{
    help("yields current nectar flow available from flowers");
    Input(minFlow).help("Minimum nectar flow (ml/min)");
    Input(maxFlow).help("Maximum nectar flow (ml/min)");
    Input(beginDate).equals(QDate(anyYear(), 1, 1));
    Input(endDate).equals(QDate(anyYear(), 12, 31));
    Input(beginTime).equals(QTime(0,0));
    Input(endTime).equals(QTime(0,0));
    Input(currentTime).imports("calendar[time]");
    Output(flow).help("Current nectar flow (ml/min)");
}

void NectarFlow::reset() {
    flow = minFlow;
}

void NectarFlow::update() {
    bool dateIsRight = isAfterOrEquals(currentDate, beginDate) &&
                       isBeforeOrEquals(currentDate, endDate),
         timeIsRight = currentTime >= beginTime &&
                       (currentTime <= endTime || endTime == QTime(0,0));
    flow = dateIsRight && timeIsRight ? maxFlow : minFlow;
}

}
