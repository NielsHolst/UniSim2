#include "exception.h"
#include "general.h"
#include "timer.h"

namespace base {

Timer::Timer(QObject *parent)
    : QObject(parent)
{
}

void Timer::addProfile(QString name) {
    Q_ASSERT(!_lookup.contains(name));
    _lookup[name] = Watch{QTime(), 0};
    _ordered << name;
}

void Timer::reset() {
    for (auto it = _lookup.begin(); it != _lookup.end(); ++it) {
        Watch &watch(*it);
        watch.second = 0;
    }
}

void Timer::start(QString name) {
    Q_ASSERT(_lookup.contains(name));
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
        rep += fullName(parent()) + "\t" +
               name + "\t" +
               QString::number(_lookup.value(name).second) +
               separator ;
    return rep;
}

QString Timer::Report::asString() const {
    return name + "\t" + QString::number(time);
}

}
