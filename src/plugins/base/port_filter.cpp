/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include <QStringList>
#include "port_filter.h"

namespace base {

#define FILTER_CASE(X) \
    case PortFilter::X: \
        s = QString(#X).toLower(); \
        break

#define FILTER_ENTRY(X) \
    _map[QString(#X).toLower()] = PortFilter::X

namespace {
    const QMap<QString, PortFilter>& portFilterMap() {
        static QMap<QString, PortFilter> _map;
        if (_map.isEmpty()) {
            FILTER_ENTRY(None);
            FILTER_ENTRY(Sum);
            FILTER_ENTRY(Mean);
            FILTER_ENTRY(Min);
            FILTER_ENTRY(Max);
            FILTER_ENTRY(End);
        }
        return _map;
    }
}

template<> QString convert(base::PortFilter filter) {
    QString s;
    switch (filter) {
        FILTER_CASE(None);
        FILTER_CASE(Sum);
        FILTER_CASE(Mean);
        FILTER_CASE(Min);
        FILTER_CASE(Max);
        FILTER_CASE(End);
    }
    return s;
}

template<> base::PortFilter convert(QString s) {
    if (s.isEmpty())
        return PortFilter::None;
    if (portFilterMap().contains(s))
        return portFilterMap().value(s);
    QString msg{"Unknown port filter. Must be one of '%1'"};
    ThrowException(msg.arg(portFilterNames().join(","))).value(s);
}

QStringList portFilterNames() {
    return QStringList(portFilterMap().keys());
}

}
