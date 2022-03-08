/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "priority_signal.h"

using namespace base;

namespace boxes {

PUBLISH(PrioritySignal)

PrioritySignal::PrioritySignal(QString name, Box *parent)
    : BaseSignal(name, parent){
    help("selects signal among child signals");
    Input(myFlags).imports("./BaseSignal::*[flagIsUp]");
    Input(mySignals).imports("./BaseSignal::*[signal]");
    Input(reverseOrder).equals(false).help("Find first signal!=0 from top (false) or bottom (true)?").unit("y|n");
}

void PrioritySignal::initialize() {
    int n = myFlags.size();
    for (int i=0; i<n; ++i) {
        FlaggedSignal fs{&myFlags[i], &mySignals[i]};
        if (reverseOrder)
            _flaggedSignals.prepend(fs);
        else
            _flaggedSignals.append(fs);
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

