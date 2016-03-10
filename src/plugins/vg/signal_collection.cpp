/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <limits>
#include <base/path.h>
#include <base/publish.h>
#include "signal_collection.h"

using std::numeric_limits;
using namespace base;

namespace vg {

PUBLISH(SignalCollection)

/*! \class SignalCollection
 * \brief Sets a signal according to a threshold value
 *
 * Inputs
 * ------
 * - _threshold_ is the threshold that the _input_ is compard with [R]
 * - _input_ is the value compard with the _threshold_ [R]
 * - _signalBelow_ is the resulting _signal_ when _input_ < _threshold_ [R]
 * - _signalAbove_ is the resulting _signal_ when _input_ >= _threshold_ [R]
 *
 */

StringMap<SignalCollection::Rule> SignalCollection::rules;

SignalCollection::SignalCollection(QString name, QObject *parent)
    : BaseSignal(name, parent){
    Input(rule).equals("max");
}

void SignalCollection::initialize() {
    QVector<BaseSignal*> children = Path("./*", this).resolveMany<BaseSignal>();
    childSignals.clear();
    for (BaseSignal *child : children) {
        childSignals << child->port("signal")->valuePtr<double>();
    }
    setRules();
}

void SignalCollection::localReset() {
    ruleDecoded = rules.seek(rule, this);
}

double SignalCollection::signal() {
    double res{0};
    switch (ruleDecoded) {
    case Min:
        res = numeric_limits<double>::max();
        for (auto childSignal : childSignals)
            res = std::min(res, *childSignal);
        break;
    case Max:
        res = numeric_limits<double>::min();
        for (auto childSignal : childSignals)
            res = std::max(res, *childSignal);
        break;
    case Sum:
        res = 0.;
        for (auto childSignal : childSignals)
            res += *childSignal;
    }
    return res;
}

void SignalCollection::setRules() {
    if (!rules.isEmpty()) return;
    rules["min"] = Min;
    rules["max"] = Max;
    rules["sum"] = Sum;
}

} //namespace

