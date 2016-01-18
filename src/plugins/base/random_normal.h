/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef RANDOM_NORMAL_H
#define RANDOM_NORMAL_H
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boxes/random.h>
#include "random_base.h"

namespace base {

class RandomNormal : public RandomBase
{
public: 
    RandomNormal(QString name, QObject *parent);
    ~RandomNormal();
    void initialize();
private:
    // Inputs
    double mean, sd;
    // Methods
    double drawValue();
    // Random number generation
    typedef boost::normal_distribution<double> Distribution;
    typedef boost::variate_generator<boxes::Random::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
