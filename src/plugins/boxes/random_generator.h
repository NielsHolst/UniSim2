#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H
#include <QObject>
#include <boost/random/mersenne_twister.hpp>

namespace boxes {

class RandomGenerator : public QObject
{
public: 
    typedef boost::mt19937 Generator;
    friend RandomGenerator::Generator* randomGenerator();
private:
    RandomGenerator();
    Generator generator;
    static RandomGenerator *theRandomGenerator;
};

RandomGenerator::Generator* randomGenerator();

} //namespace
#endif
