#include <base/publish.h>
#include "Rate_of_change_of_frond_area.h"

using namespace base;

namespace saccharina {

PUBLISH(Rateofchangeoffrondarea)

Rateofchangeoffrondarea::Rateofchangeoffrondarea(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the rate of change in frond area (Eq 1)");
    Input(u).equals(0.18).help("Specific growth rate (area), derived variable");
    Input(fronderosion).equals(0.01).help("Frond erosion");
    Input(A).equals(6.0).help("Frond area");
    Output(frondgrowthrate).help("Rate of change in nitrogen reserve");
}

void Rateofchangeoffrondarea::reset() {
   update();
}

void Rateofchangeoffrondarea::update() {
    frondgrowthrate = (u - fronderosion) * A;
}
}
