#ifndef RANDOM_POISSON_H
#define RANDOM_POISSON_H
#include <boost/random/poisson_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boxes/random_generator.h>
#include "random_base.h"

namespace base {

class RandomPoisson : public RandomBase
{
public: 
    RandomPoisson(QString name, QObject *parent);
    ~RandomPoisson();
    void initialize();
private:
    // Inputs
    double mean;
    // Methods
    double drawValue();
    // Random number generation
    typedef boost::poisson_distribution<int, double> Distribution;
    typedef boost::variate_generator<boxes::RandomGenerator::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
