#ifndef RANDOMISER_MONTE_CARLO_H
#define RANDOMISER_MONTE_CARLO_H
#include <boost/random/variate_generator.hpp>
#include <base/random_generator.h>
#include "randomiser_base.h"

namespace boxes {

class RandomiserMonteCarlo : public RandomiserBase
{
public:
    RandomiserMonteCarlo(QString name, QObject *parent);
    ~RandomiserMonteCarlo();
    int numStrata() const;
private:
    // Data
    typedef boost::variate_generator<base::RandomGenerator::Generator&, Distribution> Variate;
    Variate *variate;
    // Methods
    void inializeGenerator();
    void checkIterations(int iterations);
    double draw01();
};

}

#endif
