#ifndef DRYWEIGHT_H
#define DRYWEIGHT_H
#include <base/box.h>

namespace saccharina {

class Dryweight : public base::Box
{
public:
    Dryweight(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double kA, kN, N, Nmin, kC, C, Cmin, A;
    // Outputs
    double Wd;
};

}

#endif
