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
    double demand, resource, apparency;
    // Outputs
    double supply, resourceMortality;
};

} //namespace
#endif
