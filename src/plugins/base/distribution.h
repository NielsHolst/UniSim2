#ifndef BASE_DISTRIBUTION_H
#define BASE_DISTRIBUTION_H
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <QObject>
#include <QPair>
#include <QStringList>
#include <QVector>
#include "random_generator.h"

namespace base {

class Port;

class Distribution : public QObject
{
public:
    Distribution(QString name, QObject *parent = 0);
    virtual ~Distribution();
    void arguments(QStringList args);
    virtual void mean(double value);
    virtual void sd(double value);
    virtual void min(double value);
    virtual void max(double value);
    virtual void lowerQuantile(double value);
    virtual void upperQuantile(double value);
    double draw();
    virtual QPair<double,double> bounds() const = 0;
    virtual double inverse(double y) const = 0;

    void initialize(int numSamples);
    void port(Port *port_);
    Port* port();
protected:
    void parseNext(double *value);
    bool _isFirstDraw;
private:
    virtual void parseArguments() = 0;
    void unavailable(QString method, double value);
    QStringList _args;
    int _ixNext;
    Port *_port;
    QVector<int> _strata;
    double _y0, _dy;
    // Random number generation
    typedef boost::uniform_real<double> Uniform01;
    typedef boost::variate_generator<base::RandomGenerator::Generator&, Uniform01> Variate;
    Uniform01 *uniform01;
    Variate *variate;
};

}

#endif
