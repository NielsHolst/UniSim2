#ifndef LOGISTIC_GROWTH_H
#define LOGISTIC_GROWTH_H
#include <base/box.h>

namespace student {

class LogisticGrowth : public base::Box
{
public:
    LogisticGrowth(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initial, r, K;
    // Outputs
    double density;
};

}

#endif
