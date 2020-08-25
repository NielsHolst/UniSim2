#include <base/publish.h>
#include "Rate_of_change_in_nitrogen_reserves.h"

using namespace base;

namespace saccharina {

PUBLISH(Rateofchangeinnitrogenreserves)

Rateofchangeinnitrogenreserves::Rateofchangeinnitrogenreserves(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the rate of change in nitrogen reserves (Eq 7)");
    Input(kA).equals(0.6).help("Structural dry weight per unit area");
    Input(J).equals(0.00014).help("Nitrate uptake rate");
    Input(u).equals(0.18).help("Specific growth rate");
    Input(N).equals(0.022).help("Nitrogen reserve");
    Input(Nstruct).equals(0.01).help("Amount of nitrogen per unit dry weight of structural mass");
    Input(Factor).equals(1).help("Scaling factor of model");
    Output(nitrogenreservesrate).help("Rate of change in nitrogen reserve");
}

void Rateofchangeinnitrogenreserves::reset() {
   update();
}

void Rateofchangeinnitrogenreserves::update() {
    nitrogenreservesrate = pow(kA,-1) * J - u * (N + Nstruct) * Factor;
}
}
