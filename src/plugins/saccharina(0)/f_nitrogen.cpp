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
    Input(proportion).imports("reserves/nitrogen[proportion]");
    Input(maxProportion).imports("reserves/nitrogen[maxProportion]");
    Output(value).unit("[0;1]").help("Growth factor adjustment for nitrogen reserves");
}

void Fnitrogen::reset() {
   update();
}

void Fnitrogen::update() {
    value = proportion/maxProportion;
}

}
