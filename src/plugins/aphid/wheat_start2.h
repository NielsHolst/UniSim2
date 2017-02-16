#ifndef WHEAT_START2_H
#define WHEAT_START2_H
#include <base/box.h>

namespace aphid {

class Wheat_start2 : public base::Box
{
public:
    Wheat_start2(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Temperature;
    double Nb_plants;
    // Outputs
    double startPlants;
    bool growthAlreadyStarted;
    // Data
    double Temperature_2;
    double Temperature_3;
    double Temperature_4;
    double Temperature_5;


};

}

#endif
