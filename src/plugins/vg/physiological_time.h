/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PHYSIOLOGICAL_TIME
#define PHYSIOLOGICAL_TIME

#include <base/box.h>

namespace vg {

class PhysiologicalTime : public base::Box
{
public: 
    PhysiologicalTime(QString name, QObject *parent);
    void update();
    virtual double calcDailyTimeStep() = 0;

private:
    // Inputs
    double step, total, timeStepDays;
    bool doReset, isTicking;
};

} //namespace
#endif
