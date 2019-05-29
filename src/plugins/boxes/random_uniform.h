#ifndef RANDOM_UNIFORM_H
#define RANDOM_UNIFORM_H
#include "random_base_typed.h"

namespace boxes {

class RandomUniform : public RandomBaseTyped<double>
{
public: 
    RandomUniform(QString name, QObject *parent=nullptr);
private:
    void updateValue();
};

} //namespace
#endif
