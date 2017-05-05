#ifndef RANDOM_BINOMIAL_H
#define RANDOM_BINOMIAL_H
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <base/random_generator.h>
#include "random_base.h"

namespace boxes {

class RandomBinomial : public RandomBase<bool>
{
public: 
    RandomBinomial(QString name, QObject *parent=0);
    ~RandomBinomial();
    void doInitialize();
private:
    // Inputs
    double P;
    // Methods
    bool drawValue();
    void nextValue();
    // Random number generation
    typedef boost::uniform_real<double> Distribution;
    typedef boost::variate_generator<base::RandomGenerator::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
