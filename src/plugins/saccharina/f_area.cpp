#include <base/phys_math.h>
#include <base/publish.h>
#include "f_area.h"

using namespace base;
using phys_math::sqr;

namespace saccharina {

PUBLISH(Farea)

Farea::Farea(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the effect of frond area on growth rate (eq. 3)");
    Input(m1).equals(0.1085).help("Growth rate adjustment");
    Input(m2).equals(0.03).help("Growth rate adjustment parameter");
    Input(A0).equals(6.0).unit("dm2").help("Growth rate adjustment parameter");
    Input(A).imports("area[value]");
    Output(value).unit("[0;1]").help("Growth factor adjustment for area");
}

void Farea::reset() {
   update();
}

void Farea::update() {
    // Re-scale to max value of 1
    value = (m1*exp(-sqr(A/A0)) + m2)/(m1 + m2);
}
}
