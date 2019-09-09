/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include "general.h"
#include "proportional_signal.h"
#include <base/publish.h>

using namespace base;

namespace vg {
	
PUBLISH(ProportionalSignal)

ProportionalSignal::ProportionalSignal(QString name, QObject *parent)
    : SlidingSignal(name, parent)
{
    help("produces a signal value that responds proportionally to the input value");
}

double ProportionalSignal::slide(double proportion) {
    return proportion;
}


} //namespace

