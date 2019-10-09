/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PROPORTIONAL_SIGNAL_H
#define PROPORTIONAL_SIGNAL_H

#include <base/sliding_signal.h>

namespace boxes {

class ProportionalSignal : public base::SlidingSignal
{
public:
    ProportionalSignal(QString name, QObject *parent);
    double slide(double proportion);
};
} //namespace


#endif
