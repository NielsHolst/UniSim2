//definining headers
#ifndef PTMAX_H
#define PTMAX_H
#include <base/box.h>

namespace potato {

class PTmax: public base::Box
{
public:
    PTmax(QString name, QObject *parent);
    void reset();
    void update();
private:
    //inputs
    double Tmax, Topt, Tfloor, Tceiling;
    // Outputs //Physiologic development for minimum temperature
    double PTmax;//PTmin is the name of the output of the Physologic Development with the Temperature mini in a day
    // Data

};

}

#endif
