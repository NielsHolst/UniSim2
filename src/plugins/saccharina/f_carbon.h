#ifndef F_CARBON_H
#define F_CARBON_H
#include <base/box.h>

namespace saccharina {

class Fcarbon : public base::Box
{
public:
    Fcarbon(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double C, Cmin, Cmax;
    // Outputs
    double value;
};

}

#endif
