/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "exception.h"
#include "node.h"
#include "timer.h"

namespace base {

Timer::Timer(Node *parent)
    : _parent(parent)
{
}

void Timer::addProfile(QString name) {
    if (_lookup.contains(name))
        ThrowException("Timer already present").value(name).context(_parent);
    _lookup[name] = Watch{QElapsedTimer(), 0};
    _ordered << name;
}

void Timer::reset() {
    for (auto it = _lookup.begin(); it != _lookup.end(); ++it) {
        Watch &watch(*it);
        watch.second = 0;
    }
}

void Timer::start(QString name) {
    if (!_lookup.contains(name))
        ThrowException("No timer with that name").value(name)
                .value2(QStringList(_lookup.keys()).join(", ")).context(_parent);
    _lookup[name].first.start();
}

void Timer::stop(QString name) {
    Q_ASSERT(_lookup.contains(name));
    Watch &watch(_lookup[name]);
    watch.second += watch.first.elapsed();
}

QString Timer::report(QString separator) const {
    QString rep;
    for (QString name : _ordered)
        rep += _parent->fullName() + "\t" +
               name + "\t" +
               QString::number(_lookup.value(name).second) +
               separator ;
    return rep;
}

QString Timer::Report::asString() const {
    return name + "\t" + QString::number(time);
}

}
