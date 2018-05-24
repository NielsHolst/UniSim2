#ifndef SSI_DEV_RATE_H
#define SSI_DEV_RATE_H
#include <base/box.h>

namespace bmsb {

class SSIDevRate : public base::Box
{
public:
    SSIDevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double TPhi, dailytempC, rhoPhi, HA, HL, HH, TL, TH, R  ;

    // Data
    double numerator, denominator, devrate;
};

}

#endif
