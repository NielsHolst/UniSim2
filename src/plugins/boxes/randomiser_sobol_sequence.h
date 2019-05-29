#ifndef RANDOMISER_SOBOL_SEQUENCE_H
#define RANDOMISER_SOBOL_SEQUENCE_H
#include <boost/random/variate_generator.hpp>
#include <boost/random/sobol.hpp>
#include "randomiser_base.h"

namespace boxes {

class RandomiserSobolSequence : public RandomiserBase
{
public:
    RandomiserSobolSequence(QString name, QObject *parent);
    ~RandomiserSobolSequence();
    int numStrata() const;
private:
    // Data
    typedef boost::variate_generator<boost::random::sobol&, Distribution> Variate;
    boost::random::sobol *sobol;
    Variate *variate;
    // Methods
    void inializeGenerator();
    void checkIterations(int iterations);
    double draw01();
};

}

#endif
