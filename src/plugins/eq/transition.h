/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef TRANSITION_H
#define TRANSITION_H
#include <base/box.h>

namespace eq {

class Transition : public base::Box
{
public:
    Transition(QString name, QObject *parent);
    void reset();
    void update();
protected:
    // input
    double input, threshold, thresholdBand, minValue, maxValue, resetValue;
    bool increasingValue;
    // output
    double value;
};

} //namespace

#endif
