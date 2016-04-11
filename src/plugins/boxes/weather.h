#ifndef WEATHER_H
#define WEATHER_H
#include <base/box.h>

namespace boxes {

class Weather : public base::Box
{
public:
    Weather(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Tmin, Tmax;
    // Outputs
    double Tavg;
};

}

#endif
