#ifndef SOBOL_SEQUENCE_UNIFORM_H
#define SOBOL_SEQUENCE_UNIFORM_H
#include <boost/random/uniform_01.hpp>
#include <boost/random/variate_generator.hpp>
#include "random_base_typed.h"
#include "randomiser.h"

namespace boxes {

class SobolSequenceUniform : public RandomBaseTyped<double>
{
public: 
    SobolSequenceUniform(QString name, QObject *parent=nullptr);
    ~SobolSequenceUniform();
private:
    // Random number generation
    typedef boost::uniform_01<double> Distribution;
    typedef boost::variate_generator<Randomiser::SobolGenerator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
    // Methods
    void initalizeGenerator();
    void updateValue(int);
};

} //namespace
#endif
