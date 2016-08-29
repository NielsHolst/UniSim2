#ifndef BASE_UNIQUE_NAME_H
#define BASE_UNIQUE_NAME_H
#include <QObject>
#include <QStringList>
#include <QVector>

namespace base {

class Port;

class UniqueName
{
public:
    UniqueName(QVector<Port*> ports);
    QStringList resolve();
private:
    // Data
    QVector<Port*> _ports;
    struct Entry {
        QString name;
        int index;
        QObject *object;
    };
    QVector<Entry> _entries;
    // Methods
    void sortByName();
    bool relativePath(int index);
    void extendKey(int from, int end);
    void extendKeyByNumber(int from, int end);
};

}

#endif
