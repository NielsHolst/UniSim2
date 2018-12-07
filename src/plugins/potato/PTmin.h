//definining headers
#ifndef PHYSIOLOGIC_DEV_MIN_TEMP_H
#define PHYSIOLOGIC_DEV_MIN_TEMP_H
#include <base/box.h>
//create a namespace called student
namespace student {

class PhysiologicDevMinTemp: public base::Box
{
public:
    PhysiologicDevMinTemp(QString name, QObject *parent);
    void reset();
    void update();
private:
    //inputs
    double Tmin;
    double Topt;
    double Tfloor;
    double Tceiling;
    // Outputs //Physiologic development for minimum temperature
    double PTmin;//PTmin is the name of the output of the Physologic Development with the Temperature mini in a day
    // Data

};

}

#endif