#include <base/publish.h>
#include "plant.h"

using namespace base;

namespace conductance {

PUBLISH(Plant)

Plant::Plant(QString name, QObject *parent)
    : Box(name, parent)
{
    help("models a plant for the Conductance Model");
    Input(ZoneAreaDemandTotal).equals(1.);
    Input(initWeight).equals(2.).help("Initial plant weight at time zero (g per plant)");
    Input(A).equals(0.03).help("Coefficient in allometric relation for crown zone area: {A*w^phi}");
    Input(phi).equals(0.67).help("Exponent in allometric relation for crown zone area:  {A*w^phi}");
    Input(F).equals(0.01).help("Coefficient in allometric relation for plant leaf area: {F*w^theta}");
    Input(theta).equals(0.9).help("Exponent in allometric relation for plant leaf area: {F*w^theta}");
    Input(k).equals(0.6).help("Light extinction coefficient of foliage [0..1]");
    Input(eps).equals(1.).help("Light use efficiency (g/MJ) of global irradiation");
    Input(n).equals(20.).help("Plant density (plants per m^2)");
    Input(irradiation).imports("weather[irradiation]");
    Input(timeStepDays).imports("calendar[timeStepDays]");
    
    Output(phase).help("Growth phase: Unlimited(1), UnderCompression(2) or WeightProportional(3)");
    Output(WeightPlant).help("Plant weight (g per plant)");
    Output(WeightIncrement).help("Latest weight increment per plant (g per plant per day)");
    Output(WeightTotal).help("Total population plant weight (g/m^2 ground area available)");
    Output(ZoneAreaPlant).help("Crown zone area per plant "
             "(m^2 ground area owned per per plant)");
    Output(ZoneAreaTotal).help("Total population crown zone area "
             "(m^2 ground area owned per m^2 ground area available)");
    Output(LAPlant).help("Leaf area per plant (m^2 leaf area per plant)");
    Output(LAIInZone).help("Leaf area index within the crown zone area "
             "(m^2 leaf area per m^2 ground area owned)");
    Output(LAITotal).help("Leaf area index of whole population "
                     "(m^2 leaf area per m^2 ground area available)");
    Output(fz).help("Fraction of light intercepted [0..1]");
}

void Plant::initialize() {
    other = 0;
    QVector<Plant*> siblings = findMany<Plant>("otherSiblings::*");
    if (siblings.size() > 1)
        ThrowException("Max. 2 plants are allowed in community").value(siblings.size());
    other = siblings.isEmpty() ? 0 : siblings[0];
}

void Plant::reset() {
    WeightPlant = initWeight;
    WeightTotal = n*initWeight;
    LAIInZone = fz = LAPlant = LAITotal = WeightIncrement = 0.;
    changePhase(Unlimited);
    updateCrownZoneArea();
}

void Plant::changePhase(Phase newPhase) {
    _phase = newPhase;
    phase = int(_phase);
    Q_ASSERT(_phase == Unlimited || _phase == UnderCompression || other);
}

void Plant::update() {
    updateCrownZoneArea();
    updateLightInterception();
    updateWeight();
}

void Plant::updateCrownZoneArea() {
    switch (_phase) {
        case Unlimited:
            double zoneAreaDemand = (WeightPlant==0.) ? 0. : A*pow(WeightPlant, phi);
            if (ZoneAreaDemandTotal > 1)
            break;
        case UnderCompression:
            ZoneAreaPlant = other ? (1. - other->port("ZoneAreaTotal")->value<double>())/n : 1./n;
            break;
        case WeightProportional:
            Q_ASSERT(other);
            double totals = WeightTotal + other->port("WeightTotal")->value<double>();
            ZoneAreaPlant = (totals==0.) ? 0. : WeightPlant/totals;
    }
    ZoneAreaTotal = n*ZoneAreaPlant;
    if (ZoneAreaTotal < 0)
        ZoneAreaPlant = ZoneAreaTotal = 0.;
    else if (ZoneAreaTotal > 1) {
        ZoneAreaTotal = 1.;
        ZoneAreaPlant = 1./n;
    }
}

void Plant::updateLightInterception() {
    LAPlant = (WeightPlant==0.) ? 0. : F*pow(WeightPlant, theta);
    LAIInZone = (ZoneAreaPlant==0.) ? 0. : LAPlant/ZoneAreaPlant;
    fz = 1. - exp(-k*LAIInZone);
    LAITotal = n*LAPlant;
}

void Plant::updateWeight() {
    WeightIncrement = eps*irradiation*ZoneAreaPlant*fz*timeStepDays;
    WeightPlant += WeightIncrement;
    WeightTotal = n*WeightPlant;
}

void Plant::reUpdate() {
    WeightPlant -= WeightIncrement;
    WeightTotal = n*WeightPlant;
    update();
}

} //namespace

