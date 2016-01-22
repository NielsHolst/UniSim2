#ifndef RANDOM_UNIFORM_H
#define RANDOM_UNIFORM_H
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boxes/random_generator.h>
#include "random_base.h"

namespace base {

class RandomUniform : public RandomBase
{
public: 
    RandomUniform(QString name, QObject *parent=0);
    ~RandomUniform();
    void initialize();
private:
    // Methods
    double drawValue();
    // Random number generation
    typedef boost::uniform_real<double> Distribution;
    typedef boost::variate_generator<boxes::RandomGenerator::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif