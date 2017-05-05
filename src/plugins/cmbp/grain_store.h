#ifndef GRAIN_STORE_H
#define GRAIN_STORE_H
#include <base/box.h>

namespace cmbp {

class GrainStore : public base::Box
{
public:
    GrainStore(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double Tmin, Tmax, Toffset, mass;
    // Output
    double T;
};

}

#endif
