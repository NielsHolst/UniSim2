/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_SLIDING_SIGNAL_H
#define VG_SLIDING_SIGNAL_H

#include "base_signal.h"

namespace vg {

class SlidingSignal : public BaseSignal
{
public:
    SlidingSignal(QString name, QObject *parent);
    double computeSignal() final;
    virtual double slide(double proportion) = 0;
protected:
    double input, threshold, thresholdBand, minSignal, maxSignal;
    bool increasingSignal;
};
} //namespace


#endif
