#include <base/environment.h>
#include <base/exception.h>
#include <base/random_order.h>
#include "randomiser_stratified.h"
#include "random_base.h"
#include "randomiser_base.h"

#include <base/dialog.h>

using namespace base;

namespace boxes {

RandomBase::RandomBase(QString name, QObject *parent)
    : Box(name, parent), _randomiser(nullptr)
{
    Class(RandomBase);
    Input(P).equals(0.95).help("Range of the distribution covered by the [minValue;maxValue] range").unit("[0;1]");
    Input(useFixed).imports("ancestors::*/*<RandomiserBase>[useFixed]").help("Used fixed value instead of random values");
    Input(drawAtInitialize).imports("ancestors::*/*<RandomiserBase>[drawAtInitialize]");
    Input(drawAtReset).imports("ancestors::*/*<RandomiserBase>[drawAtReset]");
    Input(drawAtUpdate).imports("ancestors::*/*<RandomiserBase>[drawAtUpdate]");
//    Input(drawAtUserUpdate).equals(false).imports("ancestors::*<RandomiserBase>[drawAtUserUpdate]");
//    Input(drawAtUserInitialize).imports("ancestors::*<RandomiserBase>[drawAtUserInitialize]");
//    Input(drawAtUserReset).equals(false).imports("ancestors::*<RandomiserBase>[drawAtUserReset]");
}

void RandomBase::amend() {
    _order = new RandomOrder(1, this);
    RandomiserStratified *stratified = findMaybeOne<RandomiserStratified>("../*");
    if (stratified) {
        Box *child = clone("B", this);
        child->amendFamily();
    }
}

void RandomBase::initialize() {
    // Find randomiser to use
    _randomiser = findOne<RandomiserBase>("ancestors::*/*<RandomiserBase>");
    // Get number of strata
    _order->resize(_randomiser->numStrata());
    // Potential first draw
    if (drawAtInitialize)
        updateValue();
}

void RandomBase::reset() {
    if (drawAtReset) updateValue();
}

void RandomBase::update() {
    if (drawAtUpdate) updateValue();
}

void RandomBase::import() {
//    base::ComputationStep step = base::environment().computationStep();
//    if (drawAtUserInitialize && step==base::ComputationStep::Initialize)
//        updateValue(_order->next(), _order->size());
//    else if (drawAtUserReset && step==base::ComputationStep::Reset)
//        updateValue(_order->next(), _order->size());
//    else if (drawAtUserUpdate && step==base::ComputationStep::Update)
//        updateValue(_order->next(), _order->size());
}

RandomiserBase* RandomBase::randomiser() {
    if (!_randomiser)
        ThrowException("randomiser not initialized").context(this);
    return _randomiser;
}

} //namespace
