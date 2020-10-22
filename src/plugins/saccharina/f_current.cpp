#include <base/publish.h>
#include "f_current.h"

using namespace base;

namespace saccharina {

PUBLISH(Fcurrent)

Fcurrent::Fcurrent(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the effect of water current on nitrogen uptake");
    Input(uMin).equals(0.28).unit("[0;1]").help("Minimum value");
    Input(U63).equals(0.045).unit("m/s").help("Current giving a value of 0.63 when uMin=0");
    Input(U).imports("env[U]").unit("m/s").help("Water current");
    Output(value).unit("[0;1]").help("Water current scaling on nitrogen uptake");
}

void Fcurrent::reset() {
   update();
}

void Fcurrent::update() {
    value = (1. - uMin)*(1. - exp(-U/U63)) + uMin;
}

}

