#ifndef PARAM_INFECTION_H
#define PARAM_INFECTION_H
#include <base/box.h>

namespace aphid {

class Param_infection : public base::Box
{
public:
    Param_infection(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double LTimmigrants;
    double LTAdult;
    double LTNymph;
    double cadaverSurvival;

};

}

#endif
