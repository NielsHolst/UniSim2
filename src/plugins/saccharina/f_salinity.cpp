#include <base/publish.h>
#include "f_salinity.h"

using namespace base;

namespace saccharina {

PUBLISH(Fsalinity)

Fsalinity::Fsalinity(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the effect of salinity on growth rate");
    Input(S).imports("env[S]").unit("PSU").help("Environmental salinity level");
    Output(value).unit("[0;1]").help("Growth factor adjustment for salinity");
}

void Fsalinity::reset() {
   update();
}

void Fsalinity::update() {
    if (S > 25.)
        value = 1.;
    else if (S > 16.)
        value = 1. + (S - 25.)/18.;
    else
        value = S/32.;
}

}

