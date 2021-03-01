/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_UNIQUE_NAME_H
#define BASE_UNIQUE_NAME_H
#include <QObject>
#include <QStringList>
#include <QList>
#include "port_filter.h"

namespace base {

class Port;
class Track;

class UniqueName
{
public:
    UniqueName(QList<Track*> tracks);
    UniqueName(QVector<Port*> ports);
    QStringList resolve();
private:
    // Data
    struct Entry {
        QString name;
        int index;
        QObject *object;
    };
    QVector<Entry> _entries;
    int _nextEntry;
    // Methods
    void addEntry(Port *port, PortFilter filter);
    void sortByName();
    bool relativePath(int index);
    void extendKey(int from, int end);
    void extendKeyByNumber(int from, int end);
};

}

#endif
