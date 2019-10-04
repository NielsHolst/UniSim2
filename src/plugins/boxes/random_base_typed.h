/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RANDOM_BASE_TYPED_H
#define RANDOM_BASE_TYPED_H
#include <base/environment.h>
#include <base/exception.h>
#include "random_base.h"

namespace boxes {

template <class T> class RandomBaseTyped : public RandomBase
{
public: 
    RandomBaseTyped(QString name, QObject *parent);
    void update(base::Port *port);

protected:
    // Inputs
    T fixed, min, max;
    // Outputs
    T value;
    void checkInputs();
};

template <class ValueType>
RandomBaseTyped<ValueType>::RandomBaseTyped(QString name, QObject *parent)
    : RandomBase(name, parent)
{
    Input(fixed).equals(static_cast<ValueType>(0)).help("Fixed value ");
    Input(min).equals(static_cast<ValueType>(0)).help("Minimum value (included)");
    Input(max).equals(static_cast<ValueType>(1)).help("Maximum value (excluded)");
    Output(value).noReset().help("The most recently drawn value");
}

template <class ValueType>
void RandomBaseTyped<ValueType>::checkInputs() {
    if (min >= max)
        ThrowException("Boundaries must obey min<max").value(min).value2(max).context(this);
}

template <class ValueType>
void RandomBaseTyped<ValueType>::update(base::Port *port) {
    updateValue();
    port->equals(value);
}

} //namespace
#endif
