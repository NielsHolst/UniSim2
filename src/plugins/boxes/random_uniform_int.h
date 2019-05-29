#ifndef RANDOM_UNIFORM_INT_H
#define RANDOM_UNIFORM_INT_H
#include <boost/random/uniform_01.hpp>
#include <boost/random/variate_generator.hpp>
#include <base/random_generator.h>
#include "random_base_typed.h"

namespace boxes {

class RandomUniformInt : public RandomBaseTyped<int>
{
public: 
    RandomUniformInt(QString name, QObject *parent);
private:
    void updateValue();
};

} //namespace
#endif
