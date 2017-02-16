#ifndef APHID_START_SIMPLE_H
#define APHID_START_SIMPLE_H
#include <base/box.h>

namespace aphid {

class Aphid_start_simple : public base::Box
{
public:
    Aphid_start_simple(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Temperature;
    double Temperature_threshold;
    double Start_immigration;
    double Potential_immigrants;
    double Immigration_duration;

    // Outputs
    double First_immigrants;
    // Data
    double day_count;
};

}

#endif
