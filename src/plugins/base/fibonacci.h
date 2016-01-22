#ifndef FIBONACCI_H
#define FIBONACCI_H
#include <boxes/box.h>

namespace base {

class Fibonacci : public boxes::Box
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
