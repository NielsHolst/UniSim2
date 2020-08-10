﻿#include <base/publish.h>
#include "schoolfieldlow_dev_rate.h"

using namespace base;

namespace bmsb {

PUBLISH(SchoolFieldLowDevRate)

SchoolFieldLowDevRate::SchoolFieldLowDevRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily developmental rate using SchoolFieldLow model; later replace daily rate to hourly rate");
        Input(TPhi).equals(298).help("Development rate at the intrinsic optimum temperature TΦ (1/day) assuming no enzyme inactivation (days -1)");
        Input(dailytempC).equals(17).help("Absolute temperature (K) (237.16° K = 0°C)");
        Input(rhoPhi).equals(0.72).help("Length of day (hours)");
        Input(HA).equals(-12712.26).help("Enthalpy of activation of the reaction that is catalyzed by the enzyme (cal/mol)");
        Input(HL).equals(-45698.94).help("Change in enthalpy associated with low-temperature inactivation of the enzyme (cal/mol)");
        //Input(HH).equals(57331.493).help("Change in enthalpy associated with high-temperature inactivation of the enzyme (cal/mol)");
        Input(TL).equals(301.28).help("Temperature at which the enzyme is ½ active and ½ low-temperature inactive (Kelvin)");
        //Input(TH).equals(302.101).help("Intrinsic optimum temperature at which the probability of enzyme being in the active state is maximal (K)");
        Input(R).equals(1.987).help("Gas constant (1.987 cal/deg/mol)");
        Output(numerator);
        Output(denominator);
        Output(devrate).help ("devrate refers to mean development rate (1/day)");
}

void SchoolFieldLowDevRate::reset() {
        update();
}

void SchoolFieldLowDevRate::update() {
    help("hourly temperature will later replace daily temp to estimate SchoolFieldLow dev rate");
    help("Note that input parameter supplied in degree Celsius for Temperature must be converted to in degree Kelvin");
//      SchoolfieldLow model as in devRate packege in R: (p25 * (T + 273.16)/298 * exp(aa/1.987 * (1/298 - 1/(T + 273.16))))/(1 + exp(bb/1.987 * (1/cc - 1/(T + 273.16))))
//      p25 corresponds to rhoPhi, aa corresponds to HA, dd corresponds to HH and ee corresponds to TH (see; https://onlinelibrary.wiley.com/doi/full/10.1046/j.1420-9101.2001.00272.x)
        double dailytempK  = dailytempC + 273.16;
    	numerator   = ((rhoPhi) * (dailytempK/TPhi)) * (exp((HA/R) * ((1/TPhi)-(1/dailytempK))));
        denominator = (1 + (exp((HL/R) * ((1/TL) - (1/dailytempK)))));
        devrate     = numerator/denominator;
}


}

