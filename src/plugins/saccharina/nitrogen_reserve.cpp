#include <base/publish.h>
#include "nitrogen_reserve.h"

using namespace base;

namespace saccharina {

PUBLISH(NitrogenReserve)

NitrogenReserve::NitrogenReserve(QString name, QObject *parent)
    : Box(name, parent)
{
    help("maintains the the nitrogen reserve (eq. 7)");
    Input(initialValue).equals(0.015).unit("g N/g structure").help("Initial nitrogen reserve");
    Input(Nstruct).imports("biomass[Nstruct]");
    Input(kA).imports("biomass[kA]");
    Input(J).imports("nitrogenUptakeRate[value]");
    Input(u).imports("growthRate[value]");
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Output(value).unit("g N/g structure").help("Nitrogen reserve relative to structural mass");
    Output(gain).unit("g N/g structure").help("Nitrogen reserve gained from uptake");
    Output(loss).unit("g N/g structure").help("Nitrogen reserve lost to structural tissue");
}

void NitrogenReserve::reset() {
    value = initialValue;
}

void NitrogenReserve::update() {
    // [J/kA] = g N/dm2/h / (g structure/dm2) * h/d = g N / g structure / d
    double gainRate = J/kA*24.;
    gain = gainRate*timeStepDays;
    value += gain;

    // [u*(value+Nstruct)] = dm2/dm2/d * g N / g structure = g N / g structure / d
    double lossRate = u*(value + Nstruct);
    loss = lossRate*timeStepDays;
    value -= loss;
}

}
