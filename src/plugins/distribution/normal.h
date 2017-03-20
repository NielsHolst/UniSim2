#ifndef NORMAL_H
#define NORMAL_H
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <base/distribution.h>
#include <base/random_generator.h>

namespace distribution {

class normal : public base::Distribution
{
public:
    normal(QString name, QObject *parent);
    void mean(double value);
    void sd(double value);
    void lowerQuantile(double value);
    void upperQuantile(double value);
    double draw();
private:
    // Parameters
    double _mean, _sd, _lowerQuantile, _upperQuantile,
        _lowerBound, _upperBound;
    // Methods
    void parseArguments();
    void setBounds();
    // Random number generation
    typedef boost::normal_distribution<double> RndDistribution;
    typedef boost::variate_generator<base::RandomGenerator::Generator&, RndDistribution> Variate;
    RndDistribution *_distribution;
    Variate *_variate;
};

}

#endif
