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
    QVector<double> x;
    double x0,y0, x1,y1, dx, yScale;
    // output
    QVector<double> y;
    // methods
    double f(double x);
    // data
    double _a, _b;
};

}

#endif
