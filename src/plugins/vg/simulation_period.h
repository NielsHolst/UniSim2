/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SIMULATION_PERIOD_H
#define SIMULATION_PERIOD_H

#include <QDate>
#include <QTime>
#include <base/box.h>

namespace vg {
	
class SimulationPeriod : public base::Box
{
public:
    SimulationPeriod(QString name, QObject *parent);
    void reset();
private:
    // Input
    QDateTime weatherFirstDateTime;
    QDate beginDate, endDate;
    QTime beginTime, endTime;
    int timeStep;
    char timeUnit;
    // Output
    QDateTime beginDateTime;
    int steps;
    // Methods
    QDate alignYear(QDate date);
    int secsInterval();
    int secsTimeStep();
};

} //namespace

#endif
