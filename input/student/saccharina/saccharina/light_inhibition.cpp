/* Copyright 2005-2019 by Niels Holst, Aarhus University :niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/minimise.h>
#include <base/publish.h>
#include "light_inhibition.h"

using namespace base;

namespace saccharina {

PUBLISH(LightInhibition)

LightInhibition::LightInhibition(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes light inhibition parameter cf. Plat et al. (1980)");
    Input(Pmax).equals(0.001).unit("carbon/area/time").help("Maximal photosynthetic rate");
    Input(Isat).equals(200.).unit("mymol/m2/s");
    Input(alpha).equals(3.75e-5).unit("m2 s/mymol");
    Input(report).equals(true).help("Report progress on screen");
    Input(guessBeta).equals(1e-8).unit("mymol/m2/s").help("Initial guess value for beta");
    Output(beta).unit("mymol/m2/s").help("Light inhibition");
    Output(error).unit("mymol/m2/s").help("The residual error on beta");
}

void LightInhibition::reset() {
    beta = guessBeta;
}

void LightInhibition::reportBracket(double left, double right) const {
    if (report) {
        QString s("Pmax(leftBeta = %1) = %2, Pmax(rightBeta = %3) = %4");
        dialog().information(s.arg(left).arg(calcPmax(left))
                              .arg(right).arg(calcPmax(right)));
    }
}

void LightInhibition::update() {
    // Initial report
    if (report) {
        QString s("\n*** Finding beta for Pmax = %1, Isat = %2, alpha = %3 ***");
        dialog().information(s.arg(Pmax).arg(Isat).arg(alpha));
        dialog().information("Bracketing beta...");
    }

    // Find left and right beta to give opposite signs,
    // thus the optimum will be in between: leftBeta < beta < rightBeta
    double leftBeta = beta, rightBeta = beta;
    int i=0;
    const int maxIter = 10;
    if (calcPmax(beta) < Pmax) {
        // Move rightBeta farther right
        do {
            reportBracket(leftBeta, rightBeta);
            rightBeta *= 10.;
        }
        while (calcPmax(rightBeta) < Pmax && i++ < maxIter);
    }
    else {
        // Move leftBeta farther left
        do {
            reportBracket(leftBeta, rightBeta);
            leftBeta /= 10.;
        }
        while (calcPmax(leftBeta) > Pmax && i++ < maxIter);
    }
    reportBracket(leftBeta, rightBeta);

    // Check bracketing success
    if (i >= maxIter) {
        QString msg("\nPmax = %1\nIsat = %2\nalpha = %3\nLeft beta = %4\nRight beta = %5"
                    "\nLeft Pmax = %6\nRight Pmax = %7");
        msg = msg.arg(Pmax).arg(Isat).arg(alpha).arg(leftBeta).arg(rightBeta).arg(calcPmax(leftBeta)).arg(calcPmax(rightBeta));
        ThrowException("Could not bracket beta").value(msg).context(this);
    }

    // Now minimise the error
    QPair<double, double> minimum = minimise(Error(this), leftBeta, rightBeta, 1e-9, 100, this);
    beta = minimum.first;
    error = minimum.second;

    // Report result
    if (report) {
        QString s("Estimated beta = %1 with error = %2");
        dialog().information(s.arg(beta).arg(error));
    }
}

double LightInhibition::calcPmax(double altBeta) const {
    double
        a = alpha*Isat/log(1 + alpha/altBeta),
        b = alpha/(alpha + altBeta),
        c = altBeta/(alpha + altBeta);
    return a*b*pow(c, altBeta/alpha);
}

double LightInhibition::calcError(double altBeta) const {
    return fabs(calcPmax(altBeta) - Pmax);
}

Error::Error(LightInhibition *caller_)
    : caller(caller_) {
}

double Error::operator()(double const &beta) {
    return caller->calcError(beta);
}

}
