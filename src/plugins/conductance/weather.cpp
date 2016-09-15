#include <base/publish.h>
#include "weather.h"

using namespace base;

namespace conductance {

PUBLISH(Weather)

Weather::Weather(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(irradiation).equals(10).help("Global irradiation (MJ/m^2/d)");
}

} //namespace

