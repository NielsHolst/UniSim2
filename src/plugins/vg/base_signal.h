/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
    double signalReset;
    // Output
    double signal;
    bool flag, flagUp, flagDown;
};
} //namespace


#endif
