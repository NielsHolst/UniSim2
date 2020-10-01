#include <base/publish.h>
#include "nitrogen_uptake_rate.h"

using namespace base;

namespace saccharina {

PUBLISH(NitrogenUptakeRate)

NitrogenUptakeRate::NitrogenUptakeRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the relative nitrogen uptake rate (eq. 8)");
    Input(Jmax).unit("g N/dm2/h").equals(0.00014).help("Maximal relative nitrogen uptake rate");
    Input(U).imports("env[U]").unit("m/s").help("Water current speed");
    Input(U65).unit("m/s").equals(0.045).help("Constant");
    Input(a).equals(0.72).help("Constant");
    Input(b).equals(0.28).help("Constant - ensures an uptake at low currents");
    Input(N).imports("N[value]");
    Input(Nmin).imports("fnitrogen[Nmin]");
    Input(Nmax).imports("fnitrogen[Nmax]");
    Input(X).imports("env[N]").unit("mmol N/L").help("Substrate nutrient concentration");
    Input(KX).equals(0.004).unit("mmol N/L").help("Nitrate uptake half saturation constant");
    Output(value).unit("g N/dm2/h").help("Relative nitrogen uptake rate");
}

void NitrogenUptakeRate::reset() {
   update();
}

void NitrogenUptakeRate::update() {
    value = Jmax * (a*(1. - exp(-U/U65)) + b) * ((Nmax - N)/(Nmax - Nmin)) * (X/(KX + X));
}

}
