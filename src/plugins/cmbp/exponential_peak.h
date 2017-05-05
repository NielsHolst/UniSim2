#ifndef EXPONENTIAL_PEAK_H
#define EXPONENTIAL_PEAK_H
#include <base/box.h>

namespace cmbp {

class ExponentialPeak : public base::Box
{
public:
    ExponentialPeak(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double optimum, d1, d2, x;
    // Output
    double value;
};

}

#endif
