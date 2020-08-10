#ifndef SOEP_MORT_RATE_H
#define SOEP_MORT_RATE_H
#include <base/box.h>

namespace bmsb {

class SOEPMortRate : public base::Box
{
public:
    SOEPMortRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double a0, a1, a2, dailytempC;

    // Data
    double mortrate;
};

}

#endif
