#ifndef Vegetation_H
#define Vegetation_H
#include <base/box.h>

namespace pinacate {

class Vegetation : public base::Box
{
public:
    Vegetation(QString name, QObject *parent);\

    void reset();
    void update();
private:
    // Inputs
    double herbivorebiomass;
    double K;
    double scaling;
    int currentday;
    // Outputs
    double availability;
};

}

#endif
