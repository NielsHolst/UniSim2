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
    help("calculates factor for accelerate growth of small plants");
    Input(A).imports("area[value]");
    Input(a50).equals(3.).unit("dm2").help("Area at which erosion reaches 50%");
    Input(aSlope).equals(3.).help("Slope at A50");
    Input(aMax).equals(3.3);
    Output(value).unit(">=1").help("Growth factor adjustment for area");
}

void Farea::reset() {
   update();
}

void Farea::update() {
    value = (A == 0.) ? aMax : 1. + (aMax - 1.)/(1. + exp(aSlope*(log(A) - log(a50))));
}

}

