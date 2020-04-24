#ifndef FROND_EROSION_H
#define FROND_EROSION_H
#include <base/box.h>

namespace saccharina {

class Fronderosion : public base::Box
{
public:
    Fronderosion(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double A;
    double eps;
    // Outputs
    double fronderosion;
};

}

#endif
