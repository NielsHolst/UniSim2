#include <base/publish.h>
#include "carbon_reserve.h"

using namespace base;

namespace saccharina {

PUBLISH(CarbonReserve)

CarbonReserve::CarbonReserve(QString name, QObject *parent)
    : Box(name, parent)
{
    help("maintains the the carbon reserve (eq. 9)");
    Input(initialValue).equals(0.4).unit("g C/g structure").help("Initial carbon reserve");
    Input(Cstruct).imports("biomass[Cstruct]");
    Input(kA).imports("biomass[kA]");
    Input(P).imports("grossPhotosynthesis[value]");
    Input(E).imports("exudation[value]");
    Input(R).imports("respiration[value]");
    Input(u).imports("growthRate[value]");
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Input(A).imports("area[value]");
    Input(Cmin).imports("fcarbon[Cmin]");
    Output(value).unit("g C/g structure").help("Carbon reserve relative to structural mass");
    Output(gain).unit("g C/g structure").help("Carbon reserve gained from photosynthesis");
    Output(loss).unit("g C/g structure").help("Carbon reserve lost to structural tissue");
    Output(areaLost).unit("dm2").help("Area lost due to respiration");
}

void CarbonReserve::reset() {
   value = initialValue;
}

void CarbonReserve::update() {
    // [(P*(1.-E) - R)/kA*24.] = g C/dm2/h / (g structure/dm2) * h/d = g C / g structure / d
    double gainRate = (P*(1.-E) - R)/kA*24.;
    gain = gainRate*timeStepDays;
    value += gain;

    // [u*(value + Cstruct)] = dm2/dm2/d * g C/g structure = g C / g structure / d
    double lossRate = u*(value + Cstruct);
    loss = lossRate*timeStepDays;
    value -= loss;

    // Eq. between eqs. 15-16
    if (value < Cmin) {
        areaLost = A*(Cmin-value)/Cstruct;
        value = Cmin;
    }
}

}
