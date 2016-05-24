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
    QVector<double> x, dx;
    double x0,y0, x1,y1;
    // output
    QVector<double> y;
    // methods
    double f(double x, double dx);
    // data
    double _a, _b;
    bool _dxIsScalar;
};

}

#endif
