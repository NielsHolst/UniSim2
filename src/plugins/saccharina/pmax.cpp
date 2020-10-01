#include <base/phys_math.h>
#include <base/publish.h>
#include "Pmax.h"

using namespace base;
using phys_math::T0;

namespace saccharina {

PUBLISH(Pmax)

Pmax::Pmax(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the maximal, temperature-dependent photosynthesis (eq. 13)");
    Input(T).imports("env[T]");
    Input(P1).unit("g C/dm2/h").equals(0.00122).help("Maximal photosynthetic rate at T = TP1 in Kelvin");
    Input(TAP).unit("K").equals(1694.4).help("Arrhenius temperature for photosynthesis");
    Input(TP1).unit("K").equals(285).help("Reference temperature");
    Input(TAPL).unit("K").equals(27774).help("Arrhenius temperature for photosynthesis at low end of range");
    Input(TPL).unit("K").equals(271).help("Low extreme temperature");
    Input(TAPH).unit("K").equals(25924).help("Arrhenius temperature for photosynthesis at high end of range");
    Input(TPH).unit("K").equals(296).help("High extreme temperature");
    Output(value).unit("g C/dm2/h").help("Maximal photosynthesis");
}

void Pmax::reset() {
   update();
}

void Pmax::update() {
    double TK = T + T0,
           a = P1*exp(TAP /TP1 - TAP /TK),
           b =    exp(TAPL/TK  - TAPL/TPL),
           c =    exp(TAPH/TPH - TAPH/TK);

    value = a/(1 + b + c);
}

}
