/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "priority_signal.h"

using namespace base;

namespace boxes {

PUBLISH(PrioritySignal)

PrioritySignal::PrioritySignal(QString name, QObject *parent)
    : BaseSignal(name, parent){
    help("selects signal among child signals");
    Input(reverseOrder).equals(false).help("Find first signal!=0 from top (false) or bottom (true)?").unit("y|n");
}

void PrioritySignal::initialize() {
    QVector<Box*> children = findMany<Box>("./*");
    for (Box *child : children) {
        const Port *flag = child->peakPort("flagIsUp"),
                   *signal = child->peakPort("signal");
        if (flag && signal) {
            FlaggedSignal fs{flag->valuePtr<bool>(),
                             signal->valuePtr<double>()};
            if (reverseOrder)
                _flaggedSignals.prepend(fs);
            else
                _flaggedSignals.append(fs);
        }
    }
}

bool PrioritySignal::computeFlag() {
    for (const FlaggedSignal fs : _flaggedSignals) {
        if (*fs.flag)
            return true;
    }
    return false;
}

double PrioritySignal::computeSignal(bool flag) {
    if (!flag)
        return port("initialSignal")->value<bool>();
    for (const FlaggedSignal fs : _flaggedSignals) {
        if (*fs.flag)
            return *fs.signal;
    }
    Q_ASSERT(false);
    return 0.;
}


} //namespace

