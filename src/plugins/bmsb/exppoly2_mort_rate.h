#ifndef EXPPOLY2_MORT_RATE_H
#define EXPPOLY2_MORT_RATE_H
#include <base/box.h>

namespace bmsb {

class ExpPoly2MortRate : public base::Box
{
public:
    ExpPoly2MortRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a0, a1, a2;

    // Data
    double survrate, mortrate;
};

}

#endif
