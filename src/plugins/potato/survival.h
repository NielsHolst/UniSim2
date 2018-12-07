#ifndef SURVIVAL_H
#define SURVIVAL_H
#include <base/box.h>

namespace potato {

class Survival: public base::Box
{
public:
    Survival(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double senescence;
    // Outputs
    double value;
};

}
#endif
