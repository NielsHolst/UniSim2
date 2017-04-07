#ifndef RANDOM_BASE_H
#define RANDOM_BASE_H
#include <limits>
#include <base/box.h>
#include <base/environment.h>
#include <base/random_generator.h>

namespace boxes {

template <class T>
class RandomBase : public base::Box
{
public: 
    RandomBase(QString name, QObject *parent);
    void initialize() final;
    void reset() final;
    void update() final;
    void import() final;
protected:
    // Inputs
    T minValue, maxValue;
    int seedValue;
private:
    int maxTries;
    bool drawAtInitialize, drawAtUserInitialize,
        drawAtReset, drawAtUserReset,
        drawAtUpdate, drawAtUserUpdate;
    // Outputs
    T value;
    // Methods
    virtual void doInitialize() = 0;
    virtual T drawValue() = 0;
    void nextValue();
};

template <class T>
RandomBase<T>::RandomBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(RandomBase);
    Input(seedValue).help("Seed value; if this is zero a random seed value will be used");
    Input(minValue).equals(std::numeric_limits<T>::lowest()).help("Minimum value (included), maybe imposed on underlying distribution");
    Input(maxValue).equals(std::numeric_limits<T>::max()).help("Maximum value (excluded), maybe imposed on underlying distribution");
    Input(maxTries).equals(100).help("The maximum number of draws to make to fall inside lowerBund and upperBound");
    Input(drawAtInitialize).equals(false).help("Draw a value when this box is initialized?");
    Input(drawAtUserInitialize).equals(false).help("Draw a value when a user of this box is initialized?");
    Input(drawAtReset).equals(false).help("Draw a value when this box is reset?");
    Input(drawAtUserReset).equals(false).help("Draw a value when a user of this box is reset?");
    Input(drawAtUpdate).equals(false).help("Draw a value when this box is updated?");
    Input(drawAtUserUpdate).equals(false).help("Draw a value when a user of this box is updated?");
    Output(value).noReset().help("The most recently drawn value");
}

template <class T>
void RandomBase<T>::initialize() {
    int value = (seedValue==0) ? (int) std::time(0) : seedValue;
    base::seedRandomGenerator(value);
    doInitialize();
    if (drawAtInitialize) nextValue();
}

template <class T>
void RandomBase<T>::reset() {
    if (drawAtReset) nextValue();
}

template <class T>
void RandomBase<T>::update() {
    if (drawAtUpdate) nextValue();
}

template <class T>
void RandomBase<T>::import() {
    base::ComputationStep step = base::environment().computationStep();

    if (drawAtUserInitialize && step==base::ComputationStep::Initialize) nextValue();
    else if (drawAtUserReset && step==base::ComputationStep::Reset) nextValue();
    else if (drawAtUserUpdate && step==base::ComputationStep::Update) nextValue();
}

template <class T>
void RandomBase<T>::nextValue() {
    int i = 0;
    bool found;
    do {
        value = drawValue();
        if (++i == maxTries) {
            QString msg = "Max number of tries exceeded to find random value inside interval: %1 to %2";
            ThrowException(msg.arg(minValue).arg(maxValue)).value(maxTries).context(this);
        }
        found = (minValue <= value && value < maxValue);
    } while (!found);
}


} //namespace
#endif
