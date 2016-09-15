#ifndef COMMUNITY
#define COMMUNITY
#include <QVector>
#include <base/box.h>
#include "phase.h"

namespace conductance {

class Plant;

class Community  : public base::Box
{
public:
    Community(QString name, QObject *parent);
	void initialize();
	void reset();
	void update();

private:
    // methods
    void adjustTimeStep();
    bool phaseChanged();
    void updateTotalCrownZoneArea();
    bool phaseUnlimitedChanged();
    void sortPlants();
    bool phaseUnderCompressionChanged();
    void reUpdatePlants();

    // state
    Phase phase;
    double sum_sz;

    // links
    QVector<Plant*> plants;
    Plant *smaller, *larger;

};

} //namespace
#endif
