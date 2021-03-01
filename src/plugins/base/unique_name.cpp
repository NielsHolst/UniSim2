/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include <port.h>
#include <track.h>
#include "unique_name.h"

namespace base {

UniqueName::UniqueName(QList<Track *> tracks)
{
    _nextEntry = 0;
    for (Track *track : tracks)
        addEntry(track->port(), track->filter());
}

UniqueName::UniqueName(QVector<Port*> ports) {
    _nextEntry = 0;
    for (Port *port : ports)
        addEntry(port, PortFilter::None);
}

void UniqueName::addEntry(Port *port, PortFilter filter) {
//    bool hasLabel = (!port->label().isEmpty());
//    QString label = hasLabel ? port->label() : port->objectName();
    QString label = port->objectName();
    if (filter != PortFilter::None)
        label += "." + convert<QString>(filter);
    _entries << Entry{label, _nextEntry++, port};
}

QStringList UniqueName::resolve() {
    int n(_entries.size());
    bool extendMore;
    do {
        sortByName();
        int begin, end(0);
        extendMore = false;
        while (end < n) {
            begin = end;
            while(end < n &&
                  _entries.at(begin).name == _entries.at(end).name)
                ++end;
            if (end - begin > 1 && relativePath(begin)) {
                extendKey(begin, end);
                extendMore = extendMore || relativePath(begin);
            }
        }
    } while (extendMore);

    int begin, end(0);
    while (end < n) {
        begin = end;
        while(end < n &&
              _entries.at(begin).name == _entries.at(end).name)
            ++end;
        if (end - begin > 1) {
            extendKeyByNumber(begin, end);
        }
    }

    std::sort(_entries.begin(), _entries.end(),
              [](Entry a, Entry b) { return a.index < b.index; });
    QStringList names;
    for (Entry entry : _entries)
        names << entry.name;
    return names;
}

void UniqueName::sortByName() {
    std::sort(_entries.begin(), _entries.end(),
              [](Entry a, Entry b) { return a.name < b.name; });
}

bool UniqueName::relativePath(int index) {
    return _entries.at(index).name.at(0) != '/';
}

void UniqueName::extendKey(int begin, int end) {
    for (int i = begin; i < end; ++i) {
        Entry &entry(_entries[i]);
        const QObject *object = entry.object;
        Q_ASSERT(object);
        QString parentName = object->parent() ? object->parent()->objectName() : QString();
        entry.name.prepend(parentName + "/");
        entry.object = object->parent();
    }
}

void UniqueName::extendKeyByNumber(int begin, int end) {
    for (int i = begin; i < end; ++i) {
        Entry &entry(_entries[i]);
        entry.name.append("_" + QString::number(i-begin+1));
    }
}


}
