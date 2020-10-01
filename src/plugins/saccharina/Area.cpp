#include <base/publish.h>
#include "area.h"

using namespace base;

namespace saccharina {

PUBLISH(Area)

Area::Area(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the frond area (eq. 1)");
    Input(initialValue).equals(0.000005).unit("dm2").help("Initial plant area");
    Input(growthRate).imports("growthRate[value]");
    Input(lossRateErosion).imports("areaLossErosion[value]");
    Input(lossRespiration).imports("C[areaLost]");
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Input(date).imports("calendar[date]");
    Output(value).help("Frond area (dm2)");
    Output(logValue).help("Log10 of frond area");
    Output(pctGrowth).unit("% per day").help("Percentage increase");
}

void Area::reset() {
   value = initialValue;
   logValue = log10(value);
   prevDate = QDate();
   prevValue = value;
}

void Area::update() {
   value += value*(growthRate - lossRateErosion)*timeStepDays - lossRespiration;
   logValue = log10(value);
   if (prevDate.isNull()) {
       prevDate = date;
   }
   else if (date > prevDate) {
       pctGrowth = (prevValue>0.) ? 100.*(value-prevValue)/prevValue : 0.;
       prevDate = date;
       prevValue = value;
   }
}

}
