#ifndef Fecundity_H
#define Fecundity_H
#include <base/box.h>

namespace pinacate {

class Fecundity : public base::Box
{
public:
    Fecundity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double density;
    double percapita;
    double availability;
    // Outputs
    double value;
};

}

#endif
