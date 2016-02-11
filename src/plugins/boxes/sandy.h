#ifndef SANDY_H
#define SANDY_H
#include <base/box.h>

namespace boxes {

class Sandy : public base::Box
{
public:
    Sandy(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double xMin, xMax, x, yMax, a, b;
    // Outputs
    double value;
    // Data
    double C;
};

}

#endif
