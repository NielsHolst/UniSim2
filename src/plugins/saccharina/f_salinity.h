#ifndef F_SALINITY_H
#define F_SALINITY_H
#include <base/box.h>

namespace saccharina {

class Fsalinity : public base::Box
{
public:
    Fsalinity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double S;
    // Outputs
    double value;
};

}

#endif
