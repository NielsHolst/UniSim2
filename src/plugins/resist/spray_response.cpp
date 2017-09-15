#include <base/publish.h>
#include "spray_response.h"

using namespace base;

namespace resist {

PUBLISH(SprayResponse)

SprayResponse::SprayResponse(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(a).help("Sigmoid dose-response; max. mortality[0;1]");
    Input(b).help("Sigmoid dose-response; slope (negative)");
    Input(c).help("Sigmoid dose-response; coverage giving 50% response [0;1]");
    Input(coverage).help("Spray coverage [0;1]");
    Input(S).help("Selection intensity [0;1]");
    Output(ss).help("Survival of SS genotype");
    Output(sr).help("Survival of SR genotype");
    Output(rr).help("Survival of RR genotype");
}


void SprayResponse::reset() {
    ss = sr = rr = 1.;
}

void SprayResponse::update() {
    double Sminus = 1 - S,
           Splus  = 1 + S;

    ss = 1 - a*S/(1 + pow(coverage/c, b));
    sr = 1 - a*Sminus/(1 + pow(coverage/c*Splus, b*Splus));
    sr = 1 - a*Sminus*Sminus/(1 + pow(coverage/c*Splus*Splus, b*Splus*Splus));
}

}
