#ifndef RANDOMISER_BASE_H
#define RANDOMISER_BASE_H
#include <boost/random/uniform_01.hpp>
#include <boost/random/variate_generator.hpp>
#include <QVector>
#include <base/box.h>
#include <base/matrix.h>

namespace boxes {

class RandomBase;
class RandomiserBase : public base::Box
{
public:
    RandomiserBase(QString name, QObject *parent);
    void initialize() final;
    void reset() final;
    void debrief() final;
    virtual int numStrata() const = 0;
    virtual double draw01() = 0;
private:
    // Inputs
    int iteration, seed, bootstrapSize;
    bool
        useFixed,
        drawAtInitialize, drawAtUserInitialize,
        drawAtReset, drawAtUserReset,
        drawAtUpdate, drawAtUserUpdate;
    // Data
    QVector<RandomBase*> randomVariables;
    base::Matrix<double> A, B, C;
    char phase;
    bool matricesFilled;
    // Methods
    void findRandomVariables();
    void setBaseSampleSize();
    void fillMatrices();
    virtual void inializeGenerator() = 0;
    virtual void checkIterations(int iterations) = 0;
protected:
    // Inputs
    int iterations;
    bool doSensitivityAnalysis;
    // Outputs
    int numVariables;
    // Data
    typedef boost::uniform_01<double> Distribution;
    Distribution distribution;
    int N;
    // Methods
    void checkIterationsSimple(int iterations);
    RandomBase* variableB(RandomBase *A);
};

}

#endif
