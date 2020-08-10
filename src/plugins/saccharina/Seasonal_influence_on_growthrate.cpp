#include <math.h>
#include <base/publish.h>
#include "Seasonal_influence_on_growthrate.h"

using namespace base;
using namespace std;

namespace saccharina {

PUBLISH(Seasonalinfluenceongrowthrate)

Seasonalinfluenceongrowthrate::Seasonalinfluenceongrowthrate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the seasonal influence on growth rate (Eq 5)");
    Input(a1).equals(0.85).help("Photoperiodic parameter");
    Input(a2).equals(0.3).help("Photoperiodic parameter");
    Input(dayLength).imports("calendar[dayLength]");
    Input(maxDayLengthChange).equals(0.1).help("Maximum change in day length between days").unit("h");
    Output(dayLengthChange).help("Change in day length from yesterday").unit("h");
    Output(lambda).help("Scaled day length change").unit("[-1,1]");
    Output(fphoto).help("Scaling of growth rate");
}

void Seasonalinfluenceongrowthrate::reset() {
    prevDayLength = dayLength;
    update();
}

// Returns one of: -1, 0, +1
inline double signum(double val) {
    return (0. < val) - (val < 0.);
}

void Seasonalinfluenceongrowthrate::update() {
    dayLengthChange = dayLength - prevDayLength;
    prevDayLength = dayLength;
    lambda  = dayLengthChange/maxDayLengthChange;
    fphoto = a1*(1. + signum(lambda)*sqrt(abs(lambda))) + a2;
}

}
