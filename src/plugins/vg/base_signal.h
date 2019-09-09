/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_SIGNAL_H
#define BASE_SIGNAL_H

#include <base/box.h>

namespace vg {

class BaseSignal : public base::Box
{
public:
    BaseSignal(QString name, QObject *parent);
    void reset() final;
    void update() final;
    virtual void localReset() {}
    virtual double computeSignal() = 0;
private:
    // Input
    double signalReset, maxChange, timeStepSecs;
    // Output
    double signal;
    bool flag, flagUp, flagDown;
    // Data
    double _prevSignal;
    // Methods
    void limitSignal();
};
} //namespace


#endif
