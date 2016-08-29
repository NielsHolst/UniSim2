#ifndef FIBONACCI_H
#define FIBONACCI_H
#include <base/box.h>

namespace farm {

class Fibonacci : public base::Box
{
public:
    Fibonacci(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Outputs
    int value;
    // Data
    int previous;
};

}

#endif
