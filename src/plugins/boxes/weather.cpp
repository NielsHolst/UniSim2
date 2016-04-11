#include <base/exception.h>
#include <base/publish.h>
#include "weather.h"

using namespace base;

namespace boxes {

PUBLISH(Weather)

Weather::Weather(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Tmin).imports("descendants::*[Tmin]");
    Input(Tmax).imports("descendants::*[Tmax]");
    Output(Tavg);
}

void Weather::reset() {
    update();
}

void Weather::update() {
    Tavg = (Tmin + Tmax)/2.;
}

}
