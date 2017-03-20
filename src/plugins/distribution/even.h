#ifndef EVEN_H
#define EVEN_H
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <base/distribution.h>
#include <base/random_generator.h>

namespace distribution {

class even : public base::Distribution
{
public:
    even(QString name, QObject *parent);
    ~even();
    void min(double value);
    void max(double value);
    double draw();
private:
    // Parameters
    double _min, _max;
    // Methods
    void parseArguments();
    // Random number generation
    typedef boost::uniform_real<double> RndDistribution;
    typedef boost::variate_generator<base::RandomGenerator::Generator&, RndDistribution> Variate;
    RndDistribution *_distribution;
    Variate *_variate;
};

}

#endif
