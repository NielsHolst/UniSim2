#include <base/publish.h>
#include "f_carbon.h"

using namespace base;

namespace saccharina {

PUBLISH(Fcarbon)

Fcarbon::Fcarbon(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the effect of carbon reserves on growth rate (eq. 2)");
    Input(C).imports("C[value]");
    Input(Cmin).equals(0.01).unit("g N/g structure").help("Minimum carbon reserve");
    Input(Cmax).equals(0.60).unit("g N/g structure").help("Maximum carbon reserve");
    Output(value).unit("[0;1]").help("Growth factor adjustment for carbon reserves");
}

void Fcarbon::reset() {
    if (Cmin >= Cmax)
        ThrowException("Cmin must be less then Cmax").value(Cmin).value2(Cmax).context(this);
   update();
}

void Fcarbon::update() {
    // C might be slightly outside [Cmin; Cmax] due to round-off errors
    if (C < Cmin)
        value = 0.;
    else if (C < Cmax)
        value = (C - Cmin)/(Cmax - Cmin);
    else
        value = 1.;
}

}
