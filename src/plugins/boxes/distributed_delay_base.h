#ifndef DISTRIBUTED_DELAY_BASE_H
#define DISTRIBUTED_DELAY_BASE_H
#include <QObject>
#include <QVector>

namespace boxes {

class DistributedDelayBase
{
public:
    DistributedDelayBase(QObject *parent);
    void scale(double factor);
    const QVector<double>& take(double proportion);
    void put(const QVector<double> &addition);
    const double *data() const;
    double content() const;

protected:
    QVector<double> x;
    double xSum;
    QObject *parent;

private:
    QVector<double> taken;
};

}
#endif
