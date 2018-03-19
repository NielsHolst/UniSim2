/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SIMULATION_TIME_INTERVAL_H
#define SIMULATION_TIME_INTERVAL_H

#include <QDate>
#include <QTime>
#include <base/box.h>

namespace vg {
	
class SimulationTimeInterval : public base::Box
{
public:
    SimulationTimeInterval(QString name, QObject *parent);
    void reset();
private:
    // Input
    QDate beginDate, endDate;
    QTime beginTime, endTime;
    int timeStep;
    char timeUnit;
    // Output
    QDateTime beginDateTime;
    int steps;
    // Methods
    long long int secsInterval();
    long long int secsTimeStep();
};

} //namespace

#endif
