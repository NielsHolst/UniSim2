#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "coffee_summary.h"

using namespace base;
using namespace phys_math;

namespace student {

PUBLISH(CoffeeSummary)

CoffeeSummary::CoffeeSummary(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes summary of yearly harveset");
    Input(harvestTotalOutflow).help("Accumulated harvest");
    Input(skipYears).equals(3).help("How many years to skip in the beginning");
    Input(date          ).imports("calendar[date]");
    Output(average).help("Average harvest").unit("per year");
    Output(sd).help("Standard deviation of harvest").unit("per year");
}

void CoffeeSummary::reset() {
    harvests.clear();
    year = 0;
}

void CoffeeSummary::update(){
    if (date.month()==7 && date.day()==31) {
        if (year > skipYears)
            harvests.append(harvestTotalOutflow -  lastHarvest);
        ++year;
        lastHarvest = harvestTotalOutflow;
    }
 }

void CoffeeSummary::cleanup() {
    average = vector_op::average(harvests);
    double ss = 0.;
    for (double harvest : harvests)
        ss += sqr(harvest - average);
    int n = harvests.size();
    if (n < 2)
        ThrowException("Too few years in simulation").value(n).context(this);
    sd = sqrt(ss/(n-1));
}


}
