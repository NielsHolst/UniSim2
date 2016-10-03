#ifndef WEATHER_H
#define WEATHER_H

#include <base/box.h>

namespace awe {

class Weather : public base::Box
{
public:
    Weather(QString name, QObject *parent=0);
    void reset();
	void update();
private:
    // inputs
    double Tmin, max, c;
    int offset, dayOfYear;
    // outputs
    double T;
    // methods
    double temperature(int dayOfYear) const;
};


}
#endif
