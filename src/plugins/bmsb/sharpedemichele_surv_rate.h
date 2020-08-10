#ifndef SHARPEDEMICHELE_SURV_RATE_H
#define SHARPEDEMICHELE_SURV_RATE_H
#include <base/box.h>

namespace bmsb {

class SharpeDeMicheleSurvRate : public base::Box
{
public:
    SharpeDeMicheleSurvRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double  dailytempC, Phi, HA, SL, SH, HL,  HH,  R  ;

    // Data
    double numerator, denominator, survrate;
};

}

#endif
