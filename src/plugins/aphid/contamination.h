#ifndef CONTAMINATION_H
#define CONTAMINATION_H
#include <base/box.h>

namespace aphid {

class Contamination : public base::Box
{
public:
    Contamination(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double virulence_strain;//from 0 to 1
    double transmission_efficiency; //from 0 to 1
    // Outputs
    double proba_infection;


};

}

#endif
