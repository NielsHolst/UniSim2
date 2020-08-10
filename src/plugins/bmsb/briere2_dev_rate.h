#ifndef BRIERE2_DEV_RATE_H
#define BRIERE2_DEV_RATE_H
#include <base/box.h>

namespace bmsb {

class Briere2DevRate : public base::Box
{
public:
    Briere2DevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, aa, bb, Tmin, Tmax ;

    // Data
    double devrate;
};

}

#endif
