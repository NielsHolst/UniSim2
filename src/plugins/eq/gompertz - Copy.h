#ifndef GOMPERTZ_H
#define GOMPERTZ_H
#include <base/box.h>

namespace eq {

class Gompertz : public base::Box
{
public:
    Gompertz(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    double yMax, a, b, dx;
    QVector<double> x;
    // output
    QVector<double> y;
    // methods
    double f(double x);
};

}

#endif
