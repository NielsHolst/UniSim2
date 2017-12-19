#ifndef CAPACITY_H
#define CAPACITY_H
#include <base/box.h>

namespace tuta {

class Capacity : public base::Box
{
public:
    Capacity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Outputs
    double factor;
    // Input
    double pestNumber, K;
};

}

#endif
