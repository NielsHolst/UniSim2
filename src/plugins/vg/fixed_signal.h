/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_FIXED_SIGNAL_H
#define VG_FIXED_SIGNAL_H

#include "base_signal.h"

namespace vg {

class FixedSignal : public BaseSignal
{
public:
    FixedSignal(QString name, QObject *parent);
    double computeSignal();
private:
    double input;
};

} //namespace
#endif
