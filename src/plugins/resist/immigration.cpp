#include <base/publish.h>
#include "immigration.h"

using namespace base;

namespace resist {

PUBLISH(Immigration)

Immigration::Immigration(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(q).help("Frequency of resistance allele");
    Input(proportion).help("Proportion of carrying capacity [0;1]");
    Input(K).help("Carrying capacity");
    Output(ss).help("Number of SS immigrants");
    Output(sr).help("Number of SR immigrants");
    Output(rr).help("Number of RR immigrants");
}

void Immigration::reset() {
    update();
}

void Immigration::update() {
    double p = 1. - q;
    ss = p*p*K*proportion;
    sr = p*q*K*proportion;
    rr = q*q*K*proportion;
}

}
