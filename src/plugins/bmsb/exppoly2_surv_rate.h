#ifndef EXPPOLY2_SURV_RATE_H
#define EXPPOLY2_SURV_RATE_H
#include <base/box.h>

namespace bmsb {

class ExpPoly2SurvRate : public base::Box
{
public:
    ExpPoly2SurvRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a0, a1, a2;

    // Data
    double survrate;
};

}

#endif
