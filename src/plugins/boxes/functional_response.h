#ifndef FUNCTIONAL_RESPONSE
#define FUNCTIONAL_RESPONSE
#include <base/box.h>

namespace boxes {

class FunctionalResponse : public base::Box
{
public: 
    FunctionalResponse(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double demand, attacker, resource, attackRate, timeStep;
    // Outputs
    double searchRate, resourceAcquired, resourceMortality;
};

} //namespace
#endif
