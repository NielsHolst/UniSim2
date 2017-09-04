#ifndef WEATHER
#define WEATHER
#include <base/box.h>

namespace conductance {

class Weather : public base::Box
{
public:
    Weather(QString name, QObject *parent);
private:
    // input
    double irradiation;
};

} //namespace
#endif
