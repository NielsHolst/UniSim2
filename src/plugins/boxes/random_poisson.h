#ifndef RANDOM_POISSON_H
#define RANDOM_POISSON_H
#include <boost/random/poisson_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <base/random_generator.h>
#include "random_base.h"

namespace boxes {

class RandomPoisson : public RandomBase<int>
{
public: 
    RandomPoisson(QString name, QObject *parent);
    ~RandomPoisson();
    void createGenerator();
private:
    // Inputs
    double mean;
    // Methods
    int drawValue();
    // Random number generation
    typedef boost::poisson_distribution<int, double> Distribution;
    typedef boost::variate_generator<base::RandomGenerator::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
