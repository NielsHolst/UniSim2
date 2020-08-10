#ifndef SCHOOLFIELDLOW_DEV_RATE_H
#define SCHOOLFIELDLOW_DEV_RATE_H
#include <base/box.h>

namespace bmsb {

class SchoolFieldLowDevRate : public base::Box
{
public:
    SchoolFieldLowDevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double TPhi, dailytempC, rhoPhi, HA,  HL, TL, R  ;

    // Data
    double numerator, denominator, devrate;
};

}

#endif
