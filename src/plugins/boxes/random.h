#ifndef RANDOM_H
#define RANDOM_H
#include <QObject>
#include <boost/random/mersenne_twister.hpp>
#include "object_pool.h"

namespace boxes {

class Random : public QObject
{
public: 
    typedef boost::mt19937 Generator;
    Random();
    static QString id();
private:
    Generator generator;
    // Singleton
    static Random *theRandomGenerator;
    friend Random::Generator* randomGenerator();

};

inline Random::Generator* randomGenerator()
{
    if (!Random::theRandomGenerator)
        Random::theRandomGenerator = objectPool()->find<Random*>(Random::id());
    return &(Random::theRandomGenerator->generator);
}

} //namespace
#endif
