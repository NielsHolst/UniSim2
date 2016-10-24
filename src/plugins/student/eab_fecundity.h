#ifndef EAB_FECUNDITY_H
#define EAB_FECUNDITY_H
#include <base/box.h>

namespace student {

class EabFecundity : public base::Box
{
public:
    EabFecundity(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double ashState;
    // Outputs
    double perCapita;
};

}

#endif
