#ifndef BASE_RANDOM_ORDER_H
#define BASE_RANDOM_ORDER_H
#include <vector>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <QObject>
#include <QVector>
#include "random_generator.h"

namespace base {

class RandomOrder : public QObject
{
public: 
    RandomOrder(int n, QObject *parent);
    ~RandomOrder();
    void resize(int n);
    void shuffle();
    int size() const;
    int next();
    int at(int i) const;
private:
    typedef boost::uniform_int<int> Distribution;
    typedef boost::variate_generator<base::RandomGenerator::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
    std::vector<int> numbers;
    int _next;
};


} //namespace
#endif
