#include <base/publish.h>
#include "Effect_of_size_on_growthrate.h"

using namespace base;

namespace saccharina {

PUBLISH(Effectofsizeongrowthrate)

Effectofsizeongrowthrate::Effectofsizeongrowthrate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the effect of size on growth rate (Eq 3)");
    Input(m1).equals(0.1085).help("Growth rate adjustment parameter");
    Input(m2).equals(0.03).help("Growth rate adjustment parameter");
    Input(A).equals(8.0).help("Frond area, state variable");
    Input(A0).equals(6.0).help("Growth rate adjustment parameter");
    Output(farea).help("Unit is dm2");
}

void Effectofsizeongrowthrate::reset() {
   update();
}

void Effectofsizeongrowthrate::update() {
    farea = m1 * exp(-(pow(A/A0,2))) + m2;
}
}
