#include <base/phys_math.h>
#include <base/dialog.h>
#include <base/publish.h>
#include "f_nitrogen.h"

using namespace base;
using phys_math::sqr;

namespace saccharina {

PUBLISH(Fnitrogen)

Fnitrogen::Fnitrogen(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the effect of nitrogen reserves on growth rate");
    Input(proportion).imports("reserves/nitrogen[proportion]");
    Input(maxProportion).imports("reserves/nitrogen[maxProportion]");
    Input(minValue).equals(0.5).help("Minimum scaling value");
    Input(shape).equals(1.).help("Shape of curve, linear if shape=1");
    Output(value).unit("[0;1]").help("Scaling of growth rate");
}

void Fnitrogen::reset() {
   update();
}

void Fnitrogen::update() {
    value = minValue + (1.-minValue)*pow(proportion/maxProportion, shape);
}

}
