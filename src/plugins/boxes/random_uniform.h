#ifndef RANDOM_UNIFORM_H
#define RANDOM_UNIFORM_H
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <base/random_generator.h>
#include "random_base.h"

namespace boxes {

class RandomUniform : public RandomBase<double>
{
public: 
    RandomUniform(QString name, QObject *parent=nullptr);
    ~RandomUniform();
    void createGenerator();
private:
    // Methods
    double drawValue();
    // Random number generation
    typedef boost::uniform_real<double> Distribution;
    typedef boost::variate_generator<base::RandomGenerator::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
