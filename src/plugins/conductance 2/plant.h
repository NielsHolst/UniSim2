#ifndef PLANT
#define PLANT
#include <QDate>
#include <base/box.h>
#include "phase.h"

namespace conductance {

class Plant : public base::Box
{
public:
    Plant(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();

    // special methods
    void changePhase(Phase newPhase);

private:
    // methods
    void updateCrownZoneArea();
    void updateLightInterception();
    void updateWeight();

    // inputs
    double initWeight, A, phi, F, theta, k, eps, n, irradiation, timeStepDays;

    // outputs
    double ZoneAreaDemandTotal, WeightPlant, WeightIncrement, WeightTotal, ZoneAreaPlant, ZoneAreaTotal, LAIInZone, fz, LAPlant, LAITotal;
    int phase;

    // data
    Phase _phase;

    // links
    Plant *other;

};

} //namespace
#endif
