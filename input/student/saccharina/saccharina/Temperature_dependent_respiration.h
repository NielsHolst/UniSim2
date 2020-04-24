#ifndef TEMPERATURE_DEPENDENT_RESPIRATION_H
#define TEMPERATURE_DEPENDENT_RESPIRATION_H
#include <base/box.h>

namespace saccharina {

class Temperaturedepententrespiration : public base::Box
{
public:
    Temperaturedepententrespiration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double R1, TAR, TR1, T;
    // Outputs
    double respiration;
};

}

#endif
