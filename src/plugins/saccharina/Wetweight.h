#ifndef WETWEIGHT_H
#define WETWEIGHT_H
#include <base/box.h>

namespace saccharina {

class Wetweight : public base::Box
{
public:
    Wetweight(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double kA, kdw, kN, N, Nmin, kC, C, Cmin, A;
    // Outputs
    double Ww;
};

}

#endif
