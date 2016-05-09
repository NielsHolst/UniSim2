#ifndef TIMER_H
#define TIMER_H
#include <QMap>
#include <QObject>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QVector>

namespace base {

class Timer : public QObject
{
public:
    Timer(QObject *parent);
    void addProfile(QString name);
    void reset();
    void start(QString name);
    void stop(QString name);

    struct Report {
        QString name;
        int time;
        QString asString() const;
    };
    QString report(QString separator="\n") const;
private:
    typedef QPair<QTime,int> Watch;
    QMap<QString, Watch> _lookup;
    QStringList _ordered;
};

}

#endif
