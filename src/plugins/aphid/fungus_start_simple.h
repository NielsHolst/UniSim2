#ifndef FUNGUS_START_SIMPLE_H
#define FUNGUS_START_SIMPLE_H
#include <base/box.h>

namespace aphid {

class Fungus_start_simple : public base::Box
{
public:
    Fungus_start_simple(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double First_immigrants;
    double RH;
    double RH_threshold;
    double first_infection_pressure;
//The temperature is taken into account in the aphid start see if it is needed here.

    // Outputs
    double First_infection;
    // Data
    double day_count;

};

}

#endif
