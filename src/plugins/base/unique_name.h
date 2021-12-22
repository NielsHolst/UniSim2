/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_UNIQUE_NAME_H
#define BASE_UNIQUE_NAME_H
#include <QObject>
#include <QStringList>
#include <QList>

namespace base {

class Port;

class UniqueName
{
public:
    UniqueName();
    UniqueName(QVector<Port*> ports);
    UniqueName(QVector<const Port*> ports);
    QStringList resolved();
private:
    // Data
    struct Entry {
        QString name;
        int index;
        const QObject *object;
    };
    QVector<Entry> _entries;
    int _nextEntry;
    QStringList _resolved;
    bool _isResolved;
    // Methods
    void addEntry(const Port *port);
    void sortByName();
    bool relativePath(int index);
    void extendKey(int from, int end);
    void extendKeyByNumber(int from, int end);
};

}

#endif
