#ifndef FIBONACCI_H
#define FIBONACCI_H
#include <base/box.h>

namespace student {

class EabFecundity : public base::Box
{
public:
    EabFecundity(QString name, QObject *parent);
    void update();
private:
    // Input
    double ashState;
    // Outputs
    double perCapita;
};

}

#endif
