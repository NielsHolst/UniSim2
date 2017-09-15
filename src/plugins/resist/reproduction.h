#ifndef REPRODUCTION_H
#define REPRODUCTION_H
#include <base/box.h>

namespace resist {

class Reproduction : public base::Box
{
public:
    Reproduction(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double N, R, survival;
    // Outputs
    double value;
};

}

#endif
