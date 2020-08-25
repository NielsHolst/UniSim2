#include <base/dialog.h>
#include <base/publish.h>
#include "Specific_growth_rate.h"

using namespace base;

namespace saccharina {

PUBLISH(Specificgrowthrate)

Specificgrowthrate::Specificgrowthrate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the specific growthrate (Eq 2)");
    Input(farea).equals(0.03).help("Effect of size on growth rate");
    Input(fphoto).equals(0.3).help("Seasonal influence on growth rate");
    Input(ftemp).equals(6.0).help("Effect of temperature on growth rate");
    Input(fsalinity).equals(1).help("Effect of salinity on growth rate");
    Input(N).equals(0.01).help("Nitrogen reserve, relative to Ws, state variable");
    Input(Nmin).equals(0.01).help("Minimal nitrogen reserve");
    Input(C).equals(0.01).help("Carbon reserves, relative to Ws, state variable");
    Input(Cmin).equals(0.01).help("Minimal carbon reserve");
    Input(dt).imports("calendar[timeStepSecs]");
    Input(Factor).equals(1).help("Scaling factor of model");
    Output(specificgrowthrate).help("Specific growthrate");
}

void Specificgrowthrate::reset() {
   update();
}

void Specificgrowthrate::update() {
    const double roundOffError = 1e-3;
    if (N < Nmin) {
        if (Nmin - N < roundOffError)
            N = Nmin;
        else
            dialog().information(QString("Specificgrowthrate: Nmin > N, %1 > %2").arg(Nmin).arg(N));
    }
    if (C < Cmin) {
        if (Cmin - C < roundOffError)
            C = Cmin;
        else
        dialog().information(QString("Specificgrowthrate: Cmin > C, %1 > %2").arg(Cmin).arg(C));
    }
    specificgrowthrate = Factor * farea * fphoto * ftemp * fsalinity * std::min((1-Nmin/N), (1-Cmin/C));
}

}
