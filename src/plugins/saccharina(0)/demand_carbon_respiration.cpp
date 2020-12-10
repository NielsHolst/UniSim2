#include <base/phys_math.h>
#include <base/publish.h>
#include "demand_carbon_respiration.h"

using namespace base;
using phys_math::T0;

namespace saccharina {

PUBLISH(DemandCarbonRespiration)

DemandCarbonRespiration::DemandCarbonRespiration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates temperature-dependent respiration rate (eq.14)");
    Input(r1).unit("g C/dm2/h").equals(0.0002785).help("Respiration rate at T = TR1");
    Input(TAR).unit("K").equals(11033).help("Arrhenius temperature for respiration");
    Input(TR1).unit("K").equals(285).help("Reference temperature for respiration");
    Input(T).imports("env[T]");
    Input(area).imports("area[value]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(value).unit("g C/dm2/h").help("Respiration rate");
}

void DemandCarbonRespiration::reset() {
   update();
}

void DemandCarbonRespiration::update() {
    double TK = T + T0;
    value = r1 * exp(TAR/TR1 - TAR/TK)*area*timeStep/3600.;
}

}
