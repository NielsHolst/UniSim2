/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_TIMER_H
#define BASE_TIMER_H
#include <QMap>
#include <QObject>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QElapsedTimer>
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
    typedef QPair<QElapsedTimer,int> Watch;
    QMap<QString, Watch> _lookup;
    QStringList _ordered;
};

}

#endif
