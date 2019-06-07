#ifndef B_H
#define B_H
#include <base/box.h>

namespace boxes {

class B : public base::Box
{
public:
    B(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double x1, x2, x3, x4, x5,
      omega1, omega2, omega3, omega4, omega5;
    // Output
    double value;
};

}

#endif
