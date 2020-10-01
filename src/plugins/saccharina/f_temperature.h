#ifndef F_TEMPERATURE_H
#define F_TEMPERATURE_H
#include <base/box.h>

namespace saccharina {

class Ftemperature : public base::Box
{
public:
    Ftemperature(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double T;
    // Outputs
    double value;
};

}

#endif
