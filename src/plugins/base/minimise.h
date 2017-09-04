#ifndef BASE_MINIMISE_H
#define BASE_MINIMISE_H

#include <cmath>
#include <boost/math/tools/minima.hpp>
#include <QPair>
#include "exception.h"

using boost::math::tools::brent_find_minima;

namespace base {

    template <class Functor>
    QPair<double, double> minimise(Functor f, double xMin, double xMax, double precision, int maxIter, QObject *caller) {
        int bits = std::ceil(1. - std::log(precision)/std::log(2));
        std::pair<double, double> minimum;
        boost::uintmax_t _maxIter = maxIter;

        try {
            minimum = brent_find_minima<Functor, double>(f, xMin, xMax, bits, _maxIter);
        }
        catch (const std::exception& e) {
            ThrowException("Cannot minimise function in this interval")
                    .value(xMin).value2(xMax).hint(e.what()).context(caller);
        }

        if (int(_maxIter) == maxIter)
            ThrowException("Cannot minimise function. Max. iterations reached").value(maxIter);

        return qMakePair(minimum.first, minimum.second);
    }

}

#endif
