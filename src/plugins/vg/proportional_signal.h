/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_PROPORTIONAL_SIGNAL_H
#define VG_PROPORTIONAL_SIGNAL_H

#include "sliding_signal.h"

namespace vg {

class ProportionalSignal : public SlidingSignal
{
public:
    ProportionalSignal(QString name, QObject *parent);
    double slide(double proportion);
};
} //namespace


#endif
