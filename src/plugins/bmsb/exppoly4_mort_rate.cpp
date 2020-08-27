﻿#include <base/publish.h>
#include "exppoly4_mort_rate.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(ExpPoly4MortRate)

ExpPoly4MortRate::ExpPoly4MortRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily survival rate using ExpPoly4 (ExpPolynomial) model and convert to mortality rate; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (°C)");
        Input(a0).equals(-1.494e+02).help("Estimate of coefficient a0)");
        Input(a1).equals(2.923e+01).help("Estimate of coefficient a1");
        Input(a2).equals(-2.028e+00).help("Estimate of coefficient a2");
        Input(a3).equals(5.857e-02).help("Estimate of coefficient a3");
        Input(a4).equals(-5.998e-04).help("Estimate of coefficient a4");
        Output(survrate).help ("survrate refers to mean survival rate (1/day)");
        Output(mortrate).help ("mortrate refers to mean mortality rate (1/day)");
}

void ExpPoly4MortRate::reset() {
        update();
}

void ExpPoly4MortRate::update() {
    help("hourly temperature will later replace daily temp to estimate exppoly4 model based surv rate");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      ExpPoly4 model as in devRate packege in R: (rT ~ a0 + a1 * T + a2 * T^2 + a3 * T^3 + a4 * T^4)
        survrate     = 1-exp((a0) +  (a1 * dailytempC) + (a2 * pow(dailytempC,2)) + (a3 * pow(dailytempC,3)) + (a4 * pow(dailytempC,4)));
        mortrate     = 1-survrate;
}


}
