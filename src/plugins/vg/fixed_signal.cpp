/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "fixed_signal.h"

using namespace base;

namespace vg {

PUBLISH(FixedSignal)

/*! \class FixedSignal
 * \brief A signal equal to the input
 *
 *
 */

FixedSignal::FixedSignal(QString name, QObject *parent)
    : BaseSignal(name, parent){
    Input(input, 0.);
}

double FixedSignal::signal() {
    return input;
}

} //namespace

