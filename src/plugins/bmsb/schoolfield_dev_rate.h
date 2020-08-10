#ifndef SCHOOLFIELD_DEV_RATE_H
#define SCHOOLFIELD_DEV_RATE_H
#include <base/box.h>

namespace bmsb {

class SchoolFieldDevRate : public base::Box
{
public:
    SchoolFieldDevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double TPhi, dailytempC, rhoPhi, HA, HL,  HH, TL, TH, R  ;

    // Data
    double numerator, denominator, devrate;
};

}

#endif
