#ifndef SALINITY_H
#define SALINITY_H
#include <base/box.h>

namespace saccharina {

class Salinity : public base::Box
{
public:
    Salinity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double S;
    // Outputs
    double fsalinity;
};

}

#endif
