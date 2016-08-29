#ifndef BASE_HISTORY_H
#define BASE_HISTORY_H
#include <QStringList>
#include <QVector>

namespace base {

class History
{
public:
    History(int bufferSize = 100);
    void add(QString entry);
    QString previous();
    QString next();
private:
    int _bufferSize, _last, _index;
    bool _isFull;
    QVector<QString> _buffer;
    int inc(int &i);
    int decr(int &i);
};

}

#endif
