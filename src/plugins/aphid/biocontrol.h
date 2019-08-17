#ifndef BIOCONTROL_H
#define BIOCONTROL_H
#include <base/box.h>

namespace aphid {

class Biocontrol : public base::Box
{
public:
    Biocontrol(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double aphidPressureWithoutF, aphidPressureWithF,
        yieldWithoutF, yieldWithF;
    // Outputs
    double aphidPressureDifference, yieldDifference;
};

}

#endif
