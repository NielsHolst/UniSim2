#include <base/publish.h>
#include "exppoly2_surv_rate.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(ExpPoly2SurvRate)

ExpPoly2SurvRate::ExpPoly2SurvRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily survival rate using ExpPoly2 (ExpPolynomial) model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (°C)");
        Input(a0).equals(13.41368).help("Estimate of coefficient a0)");
        Input(a1).equals(-1.25998).help("Estimate of coefficient a1");
        Input(a2).equals(0.02423).help("Estimate of coefficient a2");
        Output(survrate).help ("survrate refers to mean survival rate (1/day)");
}

void ExpPoly2SurvRate::reset() {
    update();
}

void ExpPoly2SurvRate::update() {
    help("hourly temperature will later replace daily temp to estimate exppoly2 model based surv rate");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      ExpPoly2 model as in devRate packege in R: (rT ~ a0 + a1 * T + a2 * T^2)
        survrate     = 1-exp((a0) +  (a1 * dailytempC) + (a2 * pow(dailytempC,2)));
}


}

