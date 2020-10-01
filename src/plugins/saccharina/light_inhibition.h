/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LIGHT_INHIBITION_H
#define LIGHT_INHIBITION_H
#include <base/box.h>

namespace saccharina {

class LightInhibition : public base::Box
{
public:
    LightInhibition(QString name, QObject *parent);
    void reset();
    void update();
    double calcPmax(double altBeta) const;
    double calcError(double altBeta) const;
private:
    // Inputs
    double Pmax, Isat, alpha, guessBeta;
    bool report;
    // Outputs
    double beta, log10beta, error, leftBeta, rightBeta, PmaxResult;
    // Methods
    void reportBracket(QString text) const;
};

class Error {
public:
    Error(LightInhibition *caller_);
    double operator()(double const &beta);
private:
    LightInhibition *caller;
};

}
#endif
