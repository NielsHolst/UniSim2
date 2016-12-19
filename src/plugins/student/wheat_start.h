#ifndef WHEAT_START_H
#define WHEAT_START_H
#include <base/box.h>

namespace student {

class Wheat_start : public base::Box
{
public:
    Wheat_start(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Temperature;
    double Nb_plants;
    // Outputs
    double startPlants;
    // Data
    double Temperature_2;
    double Temperature_3;
    double Temperature_4;
    double Temperature_5;
    bool growthAlreadyStarted;

};

}

#endif
