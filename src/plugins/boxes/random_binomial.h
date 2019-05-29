#ifndef RANDOM_BINOMIAL_H
#define RANDOM_BINOMIAL_H
#include "random_base_typed.h"

namespace boxes {

class RandomBinomial : public RandomBaseTyped<bool>
{
public: 
    RandomBinomial(QString name, QObject *parent);
private:
    void updateValue();
};

} //namespace
#endif
