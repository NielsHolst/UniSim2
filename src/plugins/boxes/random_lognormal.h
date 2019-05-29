#ifndef RANDOM_LOGNORMAL_H
#define RANDOM_LOGNORMAL_H
#include "random_base_typed.h"

namespace boxes {

class RandomLognormal : public RandomBaseTyped<double>
{
public: 
    RandomLognormal(QString name, QObject *parent);
private:
    void updateValue();
};

} //namespace
#endif
