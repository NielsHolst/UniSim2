#ifndef FECUNDITY_H
#define FECUNDITY_H
#include <base/box.h>

namespace potato {

class Fecundity: public base::Box
{
public:
    Fecundity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double R0, senescence;
    // Outputs
    double value;
};

}
#endif
