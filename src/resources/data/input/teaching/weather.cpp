#include <base/publish.h>
#include "weather.h"

using namespace base;

namespace student {

PUBLISH(Weather)

Weather::Weather(QString name, QObject *parent)
    : Box(name, parent)
{
    help("generates temperatures by an annual sine curve");
    Input(Tmin).equals(-2.).unit("oC").help("Min. avg. temperature of the year");
    Input(Tmax).equals(22.).unit("oC").help("Max. avg. temperature of the year");
    Input(dayMin).equals(30).unit("[1;365]").help("Day of year when minimum occurs");
    Input(dayOfYear).unit("[1;365]").help("Day of year");
    Output(Tavg).unit("oC").help("Average temperature on day of year");
}

void Weather::reset() {
    update();
}

void Weather::update() {
    const double pi = 3.1416;
    double k = sin(pi*(dayOfYear-dayMin)/365.);
    Tavg = Tmin + Tmax*k*k;
}

}
