/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SIMULATION_PERIOD_H
#define SIMULATION_PERIOD_H

#include <QDate>
#include <QTime>
#include <base/box.h>

namespace boxes {
	
class SimulationPeriod : public base::Box
{
public:
    SimulationPeriod(QString name, Box *parent);
    void initialize();
    void reset();
private:
    // Input
    QDateTime begin, end, recordsBegin;
    bool alignWithRecordsYear;
    int timeStep;
    QString timeUnit;
    // Output
    QDateTime beginDateTime;
    int steps;
    // Data
    QDateTime _beginAdjusted, _endAdjusted;
};

} //namespace

#endif
