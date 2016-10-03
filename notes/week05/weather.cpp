#include <base/publish.h>
#include "weather.h"

using namespace base;

namespace student {

PUBLISH(Weather)

Weather::Weather(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Tmin).equals(-2.).help("Minimum avg. temperature of the year");
    Input(Tmax).equals(22.).help("Maximum avg. temperature of the year");
    Input(dayTmin).equals(30).help("Day of year when minimum occurs");
    Input(dayOfYear).help("Day of year [1;365]");
    Output(T);
}

void Weather::reset() {
    update();
}

void Weather::update() {
    const double pi = 3.1416;
    double k = sin(pi*(dayOfYear-dayTmin)/365.);
    T = Tmin + Tmax*k*k;
}

}
