/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/environment.h>
#include <base/exception.h>
#include <base/random_order.h>
#include "randomiser_stratified.h"
#include "random_base.h"
#include "randomiser_base.h"

#include <base/dialog.h>

using namespace base;

namespace boxes {

RandomBase::RandomBase(QString name, Box *parent)
    : Box(name, parent), _randomiser(nullptr)
{
    setClassName("RandomBase");
    Input(P).equals(0.95).help("Range of the distribution covered by the [minValue;maxValue] range").unit("[0;1]");
    Input(useFixed).imports("ancestors::*/*<RandomiserBase>[useFixed]").help("Used fixed value instead of random values");
    Input(drawAtInitialize).imports("ancestors::*/*<RandomiserBase>[drawAtInitialize]");
    Input(drawAtReset).imports("ancestors::*/*<RandomiserBase>[drawAtReset]");
    Input(drawAtUpdate).imports("ancestors::*/*<RandomiserBase>[drawAtUpdate]");
}

void RandomBase::amend() {
    _number = new RandomOrder(1, this);
    RandomiserStratified *stratified = findMaybeOne<RandomiserStratified*>("../*");
    if (stratified) {
        Box *child = clone("B", this);
        child->amendFamily();
    }
}

void RandomBase::initialize() {
    // Find randomiser to use
    _randomiser = findOne<RandomiserBase*>("ancestors::*/*<RandomiserBase>");
    // Get number of strata
    _number->resize(_randomiser->numStrata());
    // Potential first draw
    if (drawAtInitialize) {
        checkInputs("initialize");
        updateValue();
    }
}

void RandomBase::reset() {
    if (drawAtReset) {
        checkInputs("reset");
        updateValue();
    }
}

void RandomBase::update() {
    if (drawAtUpdate) {
        checkInputs("update");
        updateValue();
    }
}

void RandomBase::import() {
}

RandomiserBase* RandomBase::randomiser() {
    if (!_randomiser)
        ThrowException("randomiser not initialized").context(this);
    return _randomiser;
}

} //namespace
