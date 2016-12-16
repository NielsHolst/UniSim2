#ifndef RANDOM_LOGNORMAL_H
#define RANDOM_LOGNORMAL_H
#include <boost/random/lognormal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <base/random_generator.h>
#include "random_base.h"

namespace boxes {

class RandomLognormal : public RandomBase
{
public: 
    RandomLognormal(QString name, QObject *parent);
    ~RandomLognormal();
    void doInitialize();
private:
    // Inputs
    double mean, sd;
    // Methods
    double drawValue();
    // Random number generation
    typedef boost::lognormal_distribution<double> Distribution;
    typedef boost::variate_generator<base::RandomGenerator::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
