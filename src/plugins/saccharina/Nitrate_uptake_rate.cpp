#include <base/publish.h>
#include "Nitrate_uptake_rate.h"

using namespace base;

namespace saccharina {

PUBLISH(Nitrateuptakerate)

Nitrateuptakerate::Nitrateuptakerate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the nitrate uptake rate (Eq 8)");
    Input(Jmax).equals(0.00014).help("Maximal nitrate uptake rate");
    Input(a).equals(0.72).help("Constant");
    Input(b).equals(0.28).help("Constant - ensures an uptake at low currents");
    Input(X).equals(0.001).help("Substrate nutrient concentration, environmental variable (mmol pr L)");
    Input(KX).equals(0.004).help("Nitrate uptake half saturation constant");
    Input(N).equals(6.0).help("Nitrogen reserve, relative to Ws, state variable (g N / (g sw)");
    Input(Nmin).equals(0.01).help("Minimal nitrogen reserve");
    Input(Nmax).equals(0.022).help("Maximal nitrogen reserve");
    Input(U).equals(6.0).help("Water current speed, environmental variable (m/s)");
    Input(U0).equals(0.045).help("Constant");
    Output(Nuptakerate).help("Nitrate uptake rate (gN dm−2 h−1)");
}

void Nitrateuptakerate::reset() {
   update();
}

void Nitrateuptakerate::update() {
    Nuptakerate = Jmax * (a * (1 - exp(-U/U0)) + b) * ((Nmax - N)/(Nmax - Nmin)) * (X/(KX + X));
}
}
