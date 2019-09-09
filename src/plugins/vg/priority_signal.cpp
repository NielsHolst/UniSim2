/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "priority_signal.h"

using namespace base;

namespace vg {

PUBLISH(PrioritySignal)

PrioritySignal::PrioritySignal(QString name, QObject *parent)
    : BaseSignal(name, parent){
    help("selects signal among child signals");
    Input(reverseOrder).equals(false).help("Find first signal!=0 from top (false) or bottom (true)?").unit("y|n");
}

void PrioritySignal::initialize() {
    QVector<Box*> children = findMany<Box>("./*");
    for (Box *child : children) {
        const Port *signal = child->peakPort("signal");
        if (signal) {
            if (reverseOrder)
                _signals.prepend(signal->valuePtr<double>());
            else
                _signals.append(signal->valuePtr<double>());
        }
    }
}

double PrioritySignal::computeSignal() {
    for (const double *signal : _signals) {
        if (*signal != 0.)
            return *signal;
    }
    return 0.;
}


} //namespace

