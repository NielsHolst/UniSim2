#ifndef LOGISTIC_H
#define LOGISTIC_H
#include <base/box.h>

namespace eq {

class Logistic : public base::Box
{
public:
    Logistic(QString name, QObject *parent);
    void update();
protected:
    // input
    double x, r, K, timeStep;
    // output
    double value;
};

} //namespace

#endif
