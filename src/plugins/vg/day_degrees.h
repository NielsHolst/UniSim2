/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef DAY_DEGREES
#define DAY_DEGREES
#include "physiological_time.h"

namespace vg {

class DayDegrees : public PhysiologicalTime
{
public: 
    DayDegrees(QString name, QObject *parent=0);
    double calcDailyTimeStep();
protected:
    // Inputs
    double T0, Topt, Tmax, T;
};

} //namespace
#endif
