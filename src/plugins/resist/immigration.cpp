#include <base/publish.h>
#include "immigration.h"

using namespace base;

namespace resist {

PUBLISH(Immigration)

Immigration::Immigration(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(q).help("Frequency of resistance allele [0;1]");
    Input(proportion).help("Proportion of carrying capacity [0;1]");
    Input(K).help("Carrying capacity");
    Output(ss).help("Number of SS immigrants");
    Output(sr).help("Number of SR immigrants");
    Output(rr).help("Number of RR immigrants");
}

void Immigration::reset() {
    if (q<0 || q>1)
        Exception("'q' must be inside [0;1]").value(q).context(this);
    if (proportion<0 || proportion>1)
        Exception("'proportion' must be inside [0;1]").value(proportion).context(this);
    update();
}

void Immigration::update() {
    double p = 1. - q;
    ss = p*p*K*proportion;
    sr = p*q*K*proportion;
    rr = q*q*K*proportion;
}

}
