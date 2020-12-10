#ifndef F_AREA_H
#define F_AREA_H
#include <base/box.h>

namespace saccharina {

class Farea : public base::Box
{
public:
    Farea(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double A, a50, aSlope, aMax;
    // Outputs
    double value;
};

}

#endif
