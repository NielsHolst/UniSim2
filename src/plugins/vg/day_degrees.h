/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
