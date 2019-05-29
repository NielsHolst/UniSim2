#ifndef RANDOMISER_STRATIFIED_H
#define RANDOMISER_STRATIFIED_H
#include <boost/random/variate_generator.hpp>
#include <base/random_generator.h>
#include "randomiser_base.h"

namespace boxes {

class RandomiserStratified : public RandomiserBase
{
public:
    RandomiserStratified(QString name, QObject *parent);
    ~RandomiserStratified();
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
