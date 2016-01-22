#ifndef RANDOM_BASE_H
#define RANDOM_BASE_H
#include <base/box.h>

namespace boxes {

class RandomBase : public base::Box
{
public: 
    RandomBase(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double minValue, maxValue;
    int maxTries;
    bool drawAtInitialize, drawAtReset, drawAtUpdate;
    // Outputs
    double value;
    // Methods
    virtual double drawValue() = 0;
    void nextValue();
};

} //namespace
#endif
