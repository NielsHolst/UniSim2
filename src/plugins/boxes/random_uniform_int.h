#ifndef RANDOM_UNIFORM_INT_H
#define RANDOM_UNIFORM_INT_H
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <base/random_generator.h>
#include "random_base.h"

namespace boxes {

class RandomUniformInt : public RandomBase<int>
{
public: 
    RandomUniformInt(QString name, QObject *parent=0);
    ~RandomUniformInt();
    void doInitialize();
private:
    // Methods
    int drawValue();
    // Random number generation
    typedef boost::uniform_int<int> Distribution;
    typedef boost::variate_generator<base::RandomGenerator::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
