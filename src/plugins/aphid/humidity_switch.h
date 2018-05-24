#ifndef HUMIDITY_SWITCH_H
#define HUMIDITY_SWITCH_H
#include <base/box.h>

namespace aphid {

class Humidity_switch : public base::Box
{
public:
    Humidity_switch(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double RH_threshold;
    double RH;
    // Outputs
    bool RH_switch;

};

}

#endif
