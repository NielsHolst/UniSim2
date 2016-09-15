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
    Output(sum_sz).help("Total crown zone area of the one or two plants present "
                        "(m^2 ground area owned per m^2 ground area available)");
}

void Community::initialize() {
    plants = resolveMany<Plant>("./*");
	if (plants.isEmpty())
        ThrowException("Community has no plants");
	else if (plants.size() > 2)
        ThrowException("Max. 2 plants are allowed in community").value(plants.size());
}

void Community::reset() {
    phase = Unlimited;
    smaller = larger = 0;

    updateTotalCrownZoneArea();
    if (sum_sz > 1) 
        ThrowException("Total crown zone area must be < 1 m2 on day 0").value(sum_sz);
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
    sum_sz = 0.;
    for (int i = 0; i < plants.size(); ++i)
        sum_sz += plants[i]->port("total_sz")->value<double>();
}

bool Community::phaseUnlimitedChanged() {
    bool availableAreaExhausted = sum_sz >= 1.;
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

    bool firstIsSmaller = plants[0]->port("weight")->value<double>() <
                          plants[1]->port("weight")->value<double>();
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
    bool plantsAreEven = smaller->port("Lz")->value<double>() >=
                         larger->port("Lz")->value<double>();
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

