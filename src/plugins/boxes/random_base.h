#ifndef RANDOM_BASE_H
#define RANDOM_BASE_H
#include <base/box.h>

namespace boxes {

class RandomBase : public base::Box
{
public: 
    RandomBase(QString name, QObject *parent);
    void initialize() final;
    void reset() final;
    void update() final;
private:
    // Inputs
    double minValue, maxValue;
    int maxTries;
    bool drawAtReset, drawAtUpdate;
    // Outputs
    double value;
    // Methods
    virtual void doInitialize() = 0;
    virtual double drawValue() = 0;
    void nextValue();
};

} //namespace
#endif
