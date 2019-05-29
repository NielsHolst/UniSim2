#ifndef RANDOMISER_H
#define RANDOMISER_H
#include <boost/random/sobol.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/variate_generator.hpp>
#include <base/box.h>

namespace boxes {

class Randomiser : public base::Box
{
public:
    enum Method{MonteCarlo, Stratified, SobolSequence};
    typedef boost::random::sobol SobolGenerator;

    Randomiser(QString name, QObject *parent);
    ~Randomiser();
    void initialize();
    static SobolGenerator* sobolGenerator();
private:
    // Inputs
    QString method;
    int seed, numStrata;
    bool
        drawAtInitialize, drawAtUserInitialize,
        drawAtReset, drawAtUserReset,
        drawAtUpdate, drawAtUserUpdate;
    // Outputs
    int methodAsInt, numVariables, numIterations;
    // Data
    boost::uniform_01<double> distribution;

    Method _method;
    static SobolGenerator *_sobolGenerator;
    int _numRandomVariables, _numSobolVariables;
    // Methods
    void setMethod();
    void setNumVariables();
    void setNumIterations();
    void initSobolGenerator();
};

}

#endif
