#ifndef RANDOM_LOGNORMAL_H
#define RANDOM_LOGNORMAL_H
#include <boost/random/lognormal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boxes/random.h>
#include "random_base.h"

namespace base {

class RandomLognormal : public RandomBase
{
public: 
    RandomLognormal(QString name, QObject *parent);
    ~RandomLognormal();
    void initialize();
private:
    // Inputs
    double mean, sd;
    // Methods
    double drawValue();
    // Random number generation
    typedef boost::lognormal_distribution<double> Distribution;
    typedef boost::variate_generator<boxes::Random::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
