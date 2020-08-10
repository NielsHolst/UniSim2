#include <base/dialog.h>
#include <base/publish.h>
#include <base/test_num.h>
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
    if (TestNum::gt(Nmin,N))
        dialog().information(QString("Specificgrowthrate: Nmin > N, %1 > %2").arg(Nmin).arg(N));
    if (TestNum::gt(Cmin,C))
        dialog().information(QString("Specificgrowthrate: Cmin > C, %1 > %2").arg(Cmin).arg(C));
    specificgrowthrate = Factor * farea * fphoto * ftemp * std::min((1-Nmin/N), (1-Cmin/C));
}

}
