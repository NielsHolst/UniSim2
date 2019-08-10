#ifndef RANDOM_BASE_TYPED_H
#define RANDOM_BASE_TYPED_H
#include <base/environment.h>
#include "random_base.h"

namespace boxes {

template <class T> class RandomBaseTyped : public RandomBase
{
public: 
    RandomBaseTyped(QString name, QObject *parent);
protected:
    // Inputs
    T fixed, min, max;
    // Outputs
    T value;
};

template <class ValueType>
RandomBaseTyped<ValueType>::RandomBaseTyped(QString name, QObject *parent)
    : RandomBase(name, parent)
{
    Input(fixed).equals(static_cast<ValueType>(0)).help("Fixed value ");
    Input(min).equals(static_cast<ValueType>(0)).help("Minimum value (included)");
    Input(max).equals(static_cast<ValueType>(1)).help("Maximum value (included for integers only)");
    Output(value).noReset().help("The most recently drawn value");
}

} //namespace
#endif