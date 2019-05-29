#include <base/exception.h>
#include <base/publish.h>
#include <base/random_generator.h>
#include "randomiser.h"
#include "random_base.h"

#include <base/dialog.h>

using namespace base;

namespace boxes {
Randomiser::SobolGenerator *Randomiser::_sobolGenerator = nullptr;

PUBLISH(Randomiser)

Randomiser::Randomiser(QString name, QObject *parent)
    : Box(name, parent)
{
    help("organises random number generation");
    Input(method).equals("MonteCarlo").help("Method for random number generation)")
                 .unit("MonteCarlo|Stratified|SobolSequence");
    Input(numStrata).equals(1).help("Number of strata sampled in the distribution of each variable");
    Input(seed).help("Seed for random numbers; if this is zero a random seed value will be used");
    Input(drawAtInitialize).equals(false).help("Draw a value when a box is initialized?");
    Input(drawAtUserInitialize).equals(false).help("Draw a value when a user of a box is initialized?");
    Input(drawAtReset).equals(true).help("Draw a value when a box is reset?");
    Input(drawAtUserReset).equals(false).help("Draw a value when a user of a box is reset?");
    Input(drawAtUpdate).equals(false).help("Draw a value when a box is updated?");
    Input(drawAtUserUpdate).equals(false).help("Draw a value when a user of as box is updated?");
    Output(methodAsInt).help("Method translated to integer").unit("[0;2]");
    Output(numVariables).help("Number of variables which are randomised").unit("[0;inf]");
    Output(numIterations).help("Number of iterations needed for randomisation").unit("[1;inf]");
}

Randomiser::~Randomiser() {
    delete _sobolGenerator;
    _sobolGenerator = nullptr;
}

Randomiser::SobolGenerator* Randomiser::sobolGenerator() {
    if (!_sobolGenerator)
        ThrowException("Missing Sobol generator in Randomiser box");
    return _sobolGenerator;
}

void Randomiser::initialize() {
    // Set random seed
    int theSeed = (seed==0) ? static_cast<int>(std::time(nullptr)) : seed;
    base::seedRandomGenerator(theSeed);

    setMethod();
    setNumVariables();
    setNumIterations();
    if (_method == SobolSequence)
        initSobolGenerator();
}

void Randomiser::setMethod() {
    QString s = method.toLower();
    if (s == "montecarlo")
        _method = MonteCarlo;
    else if (s == "stratified")
        _method = Stratified;
    else if (s == "sobolsequence")
        _method = SobolSequence;
    else
        ThrowException("Unknown method").value(method);
    methodAsInt = static_cast<int>(_method);
}

void Randomiser::setNumVariables(){
    QVector<RandomBase*> randomVariables = findMany<RandomBase>("*<RandomBase>");
    numVariables = randomVariables.size();
    _numRandomVariables =_numSobolVariables = 0;
    for (RandomBase *variable : randomVariables) {
        if (variable->className().contains("Random"))
            ++_numRandomVariables;
        if (variable->className().contains("Sobol"))
            ++_numSobolVariables;
    }
    if (_method == SobolSequence) {
        if (_numRandomVariables > 0)
            ThrowException("Random boxes cannot be used with randomiser method=SobolSequence").context(this);
    }
    else {
        if (_numSobolVariables > 0)
            ThrowException("Sobol boxes can only be used with randomiser method=SobolSequence").context(this);
    }
}

void Randomiser::setNumIterations() {
    switch(static_cast<Method>(methodAsInt)) {
        case MonteCarlo:
        case Stratified:
            numIterations = numStrata;
        break;
        case SobolSequence:
            numIterations = (numVariables+2)*numStrata;
    }
}

void Randomiser::initSobolGenerator() {
    dialog().information("initSobolGenerator A");
    if (numVariables < 1)
        ThrowException("Randomiser needs at least one random variable").value(numVariables).context(this);
    dialog().information("initSobolGenerator B");
    delete _sobolGenerator;
    dialog().information("initSobolGenerator C");
    _sobolGenerator = new SobolGenerator(static_cast<std::size_t>(numVariables));
    dialog().information("initSobolGenerator Z");
}

}
