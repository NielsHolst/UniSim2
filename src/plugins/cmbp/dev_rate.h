#ifndef DEV_RATE_H
#define DEV_RATE_H
#include <base/box.h>

namespace cmbp {

class DevRate : public base::Box
{
public:
    DevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double b1, b2, b3, b4, T;
    // Output
    double value;
};

}

#endif
