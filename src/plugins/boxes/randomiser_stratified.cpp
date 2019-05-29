#include <base/exception.h>
#include <base/publish.h>
#include <base/random_generator.h>
#include "randomiser_stratified.h"
#include "random_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomiserStratified)

RandomiserStratified::RandomiserStratified(QString name, QObject *parent)
    : RandomiserBase(name, parent), variate(nullptr)
{
    help("draws random numbers [0;1) by stratified method");
}

RandomiserStratified::~RandomiserStratified() {
    delete variate;
    variate = nullptr;
}

void RandomiserStratified::inializeGenerator() {
    delete variate;
    variate = new Variate(*randomGenerator(), distribution);
}

void RandomiserStratified::checkIterations(int iterations) {
    if (doSensitivityAnalysis)
        checkIterationsSimple(iterations);
}

int RandomiserStratified::numStrata() const {
    return doSensitivityAnalysis ? 2*N : iterations;
}

double RandomiserStratified::draw01() {
    return (*variate)();
}

}
