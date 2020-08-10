#ifndef SCHOOLFIELDHIGH_DEV_RATE_H
#define SCHOOLFIELDHIGH_DEV_RATE_H
#include <base/box.h>

namespace bmsb {

class SchoolFieldHighDevRate : public base::Box
{
public:
    SchoolFieldHighDevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double TPhi, dailytempC, rhoPhi, HA,  HH, TH, R  ;

    // Data
    double numerator, denominator, devrate;
};

}

#endif
