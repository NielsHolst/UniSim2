#ifndef EXPPOLY4_SURV_RATE_H
#define EXPPOLY4_SURV_RATE_H
#include <base/box.h>

namespace bmsb {

class ExpPoly4SurvRate : public base::Box
{
public:
    ExpPoly4SurvRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a0, a1, a2, a3, a4  ;

    // Data
    double survrate;
};

}

#endif