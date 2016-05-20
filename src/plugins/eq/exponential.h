#ifndef EXPONENTIAL_H
#define EXPONENTIAL_H
#include <base/box.h>

namespace eq {

class Exponential : public base::Box
{
public:
    Exponential(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    QVector<double> x;
    double r, dt, yMax;
    // output
    QVector<double> y;
    // methods
    double f(double x);
};

}

#endif
