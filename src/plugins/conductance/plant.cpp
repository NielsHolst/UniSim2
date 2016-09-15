#include <base/publish.h>
#include "plant.h"

using namespace base;

namespace conductance {

PUBLISH(Plant)

Plant::Plant(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initWeight).equals(2.).help("Initial plant weight at time zero (g per plant)");
    Input(initDay).help(
                 "Day of month when growth begins and weight is set to initWeight. "
                 "Use together with initMonth. If initDay or initMonth is zero,"
                 "they are not used and growth will start on day 1 in the simulation");
    Input(initMonth).help("See initDay");
    Input(A).equals(0.03).help("Coefficient in allometric relation for crown zone area: {A*w^phi}");
    Input(phi).equals(0.67).help("Exponent in allometric relation for crown zone area:  {A*w^phi}");
    Input(F).equals(0.01).help("Coefficient in allometric relation for plant leaf area: {F*w^theta}");
    Input(theta).equals(0.9).help("Exponent in allometric relation for plant leaf area: {F*w^theta}");
    Input(k).equals(0.6).help("Light extinction coefficient of foliage [0..1]");
    Input(eps).equals(1.).help("Light use efficiency (g/MJ) of global irradiation");
    Input(n).equals(20.).help("Plant density (plants per m^2)");
    Input(irradiation).imports("weather[irradiation]");
    Input(today).imports("calendar[date]");
    
    Output(weight).help("Plant weight (g per plant)");
    Output(totalWeight).help("Total population plant weight (g/m^2 ground area available)");
    Output(sz).help("Crown zone area per plant "
             "(m^2 ground area owned per per plant)");
    Output(total_sz).help("Total population crown zone area "
             "(m^2 ground area owned per m^2 ground area available)");
    Output(Lz).help("Leaf area index within the crown zone area "
             "(m^2 leaf area per m^2 ground area owned)");
    Output(fz).help("Fraction of light intercepted [0..1]");
    Output(LA_per_plant).help("Leaf area per plant (m^2 leaf area per plant)");
    Output(dweight).help("Latest increment in plant weight over time step dt (g per plant per day)");
    Output(lai).help("Leaf area index of whole population "
                     "(m^2 leaf area per m^2 ground area available)");
}

void Plant::initialize() {
    other = 0;
    QVector<Plant*> siblings = resolveMany<Plant>("otherSiblings::*");
    if (siblings.size() > 1)
        ThrowException("Max. 2 plants are allowed in community").value(siblings.size());
    other = siblings.isEmpty() ? 0 : siblings[0];
}

void Plant::reset() {
    weight = startAtOnce() ? initWeight : 0.;
    totalWeight = n*initWeight;
    Lz = fz = LA_per_plant = lai = dweight = 0.;
    changePhase(Unlimited);
    updateCrownZoneArea();
}

bool Plant::startAtOnce() const {
    return initDay==0 || initMonth==0;
}

void Plant::changePhase(Phase newPhase) {
    phase = newPhase;
    Q_ASSERT(phase == Unlimited || phase == UnderCompression || other);
}

void Plant::update() {
    if (startNow())
        weight = initWeight;
    updateCrownZoneArea();
    updateLightInterception();
    updateWeight();
}

bool Plant::startNow() const {
    return (today.day()==initDay && today.month()==initMonth);
}

void Plant::updateCrownZoneArea() {
    switch (phase) {
        case Unlimited:
        sz = (weight==0.) ? 0. : A*pow(weight, phi);
            break;
        case UnderCompression:
            sz = other ? (1. - other->port("total_sz")->value<double>())/n : 1./n;
            break;
        case WeightProportional:
            Q_ASSERT(other);
            double totals = totalWeight + other->port("totalWeight")->value<double>();
            sz = (totals==0.) ? 0. : weight/totals;
    }
    total_sz = n*sz;
    if (total_sz < 0)
        sz = total_sz = 0.;
    else if (total_sz > 1) {
        total_sz = 1.;
        sz = 1./n;
    }
}

void Plant::updateLightInterception() {
    LA_per_plant = (weight==0.) ? 0. : F*pow(weight, theta);
    Lz = (sz==0.) ? 0. : LA_per_plant/sz;
    fz = 1. - exp(-k*Lz);
    lai = n*LA_per_plant;
}

void Plant::updateWeight() {
    dweight = eps*irradiation*sz*fz;
    weight += dweight;
    totalWeight = n*weight;
}

void Plant::reUpdate() {
    weight -= dweight;
    totalWeight = n*weight;
    update();
}

} //namespace

