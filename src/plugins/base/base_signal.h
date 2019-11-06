/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk]. 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BASE_SIGNAL_H
#define BASE_BASE_SIGNAL_H

#include "box.h"

namespace base {

class BaseSignal : public base::Box
{
public:
    BaseSignal(QString name, QObject *parent);
    void reset() final;
    void update() final;
    virtual void localReset() {}
    virtual double computeSignal(bool flag) = 0;
    virtual bool computeFlag() = 0;
private:
    // Input
    double initialSignal, maxChange, timeStepSecs;
    bool initialFlag;
    // Output
    double signal;
    bool flagIsUp, flagJustRaised, flagJustLowered;
    // Data
    double _prevSignal;
    // Methods
    void limitSignal();
};
} //namespace


#endif
