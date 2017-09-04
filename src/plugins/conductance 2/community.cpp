#include <base/exception.h>
#include <base/publish.h>
#include "community.h"
#include "plant.h"

using namespace base;

namespace conductance {

PUBLISH(Community)

Community::Community(QString name, QObject *parent)
    : Box(name, parent)
{
    help("models a community of one or two plants in the Conductance Model");
    Output(ZoneAreaTotal).help("Total crown zone area of the plant populations "
                        "(m^2 ground area owned per m^2 ground area available)");
}

void Community::initialize() {
    plants = findMany<Plant>("./*");
	if (plants.isEmpty())
        ThrowException("Community has no plants");
	else if (plants.size() > 2)
        ThrowException("Max. 2 plants are allowed in community").value(plants.size());
}

void Community::reset() {
    phase = Unlimited;
    smaller = larger = 0;

    updateTotalCrownZoneArea();
    if (ZoneAreaTotal > 1)
        ThrowException("Total crown zone area must be < 1 m2 on day 0").value(ZoneAreaTotal);
}

void Community::update() {
    while (phaseChanged())
        reUpdatePlants();
}

bool Community::phaseChanged() {
    updateTotalCrownZoneArea();
    if (phase == Unlimited)
        return phaseUnlimitedChanged();
    else if (phase == UnderCompression && plants.size() == 2)
        return phaseUnderCompressionChanged();
    return false;
}

void Community::updateTotalCrownZoneArea() {
    ZoneAreaTotal = 0.;
    for (int i = 0; i < plants.size(); ++i)
        ZoneAreaTotal += plants[i]->port("ZoneAreaTotal")->value<double>();
}

bool Community::phaseUnlimitedChanged() {
    bool availableAreaExhausted = ZoneAreaTotal >= 1.;
    if (availableAreaExhausted) {
        sortPlants();
        smaller->changePhase(UnderCompression);
        phase = UnderCompression;
        return true;
    }
    return false;
}

void Community::sortPlants() {
    bool onlyOnePlant = plants.size() == 1;
    if (onlyOnePlant) {
        smaller = plants[0];
        larger = 0;
        return;
    }

    bool firstIsSmaller = plants[0]->port("WeightPlant")->value<double>() <
                          plants[1]->port("WeightPlant")->value<double>();
    if (firstIsSmaller) {
        smaller = plants[0];
        larger = plants[1];
    }
    else {
        smaller = plants[1];
        larger = plants[0];
    }
}

bool Community::phaseUnderCompressionChanged() {
    bool plantsAreEven = smaller->port("LAIInZone")->value<double>() >=
                         larger->port("LAIInZone")->value<double>();
    if (plantsAreEven)
    {
        smaller->changePhase(WeightProportional);
        larger->changePhase(WeightProportional);
        phase = WeightProportional;
        return true;
    }
    return false;
}

void Community::reUpdatePlants() {
    for (int i = 0; i < plants.size(); ++i)
        plants[i]->reUpdate();
}

} //namespace

