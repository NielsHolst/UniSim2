#ifndef ZIGMOID_H
#define ZIGMOID_H
#include <base/box.h>

namespace eq {

class Zigmoid : public base::Box
{
public:
    Zigmoid(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    double xMin, xMax, yMax, dx;
    QVector<double> x;
    // output
    QVector<double> y;
    // methods
    double f(double x);
};

}

#endif
