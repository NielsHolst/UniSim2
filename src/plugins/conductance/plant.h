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
    void reUpdate();

private:
    // methods
    bool startAtOnce() const;
    bool startNow() const;
    void updateCrownZoneArea();
    void updateLightInterception();
    void updateWeight();

    // inputs
    double initWeight, A, phi, F, theta, k, eps, n, irradiation;
    int initDay, initMonth;
    QDate today;

    // outputs
    double weight, dweight, totalWeight, sz, total_sz, Lz, fz, LA_per_plant, lai;
    int _phase;
    Phase phase;

    // links
    base::Box *weather, *calendar;
    Plant *other;

};

} //namespace
#endif
