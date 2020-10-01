#include <base/dialog.h>
#include <base/publish.h>
#include "f_nitrogen.h"

using namespace base;

namespace saccharina {

PUBLISH(Fnitrogen)

Fnitrogen::Fnitrogen(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the effect of nitrogen reserves on growth rate (eq. 2)");
    Input(N).imports("N[value]");
    Input(Nmin).equals(0.01).unit("g N/g structure").help("Minimum nitrogen reserve");
    Input(Nmax).equals(0.02).unit("g N/g structure").help("Maximum nitrogen reserve");
    Output(value).unit("[0;1]").help("Growth factor adjustment for nitrogen reserves");
}

void Fnitrogen::reset() {
   if (Nmin >= Nmax)
       ThrowException("Nmin must be less then Nmax").value(Nmin).value2(Nmax).context(this);
   update();
}

void Fnitrogen::update() {
    // N might be slightly outside [Nmin; Nmax] due to round-off errors
    if (N < Nmin)
        value = 0.;
    else if (N < Nmax)
        value = (N - Nmin)/(Nmax - Nmin);
    else
        value = 1.;
}

}
