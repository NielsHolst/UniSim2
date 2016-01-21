#ifndef HISTORY_H
#define HISTORY_H
#include <QStringList>
#include <QVector>

namespace console {

class History
{
public:
    History(int bufferSize = 100);
    void add(QString entry);
    QStringList entry(int index);
private:
    int _bufferSize, _begin, _end;
    QVector<QStringList> _buffer;
    int inc(int &i);
};

}

#endif
