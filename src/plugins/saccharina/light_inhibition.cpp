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
    help("calculates light inhibition parameter beta (eq. 12)");
    Input(Pmax).imports("pmax[value]");
    Input(Isat).equals(200.).unit("micromol/m2/s");
    Input(alpha).equals(3.75e-5).unit("g C/dm2/h per micromol/m2/s").help("Photosynthetic effectivity");
    Input(report).equals(false).help("Report progress on screen?");
    Input(guessBeta).equals(1e-8).unit("micromol/m2/s").help("Initial guess value for beta");
    Output(beta).unit("micromol/m2/s").help("Light inhibition");
    Output(log10beta).help("Log10 of beta");
    Output(error).unit("micromol/m2/s").help("The residual error on beta");
    Output(leftBeta).unit("micromol/m2/s").help("Lower limit for search window");
    Output(rightBeta).unit("micromol/m2/s").help("Upper limit for search window");
    Input(PmaxResult).unit("g C/dm2/h").help("Resulting maximal photosynthesis");
}

void LightInhibition::reset() {
    beta = guessBeta;
    log10beta = log10(beta);
}

void LightInhibition::reportBracket(QString text) const {
    if (report) {
        QString s("%1: Pmax(leftBeta = %2) = %3, Pmax(rightBeta = %4) = %5");
        dialog().information(s.arg(text)
                              .arg(leftBeta).arg(calcPmax(leftBeta))
                              .arg(rightBeta).arg(calcPmax(rightBeta)));
    }
}

void LightInhibition::update() {
    // Initial report
    if (report) {
        QString s("\n*** Finding beta for Pmax = %1, Isat = %2, alpha = %3 ***");
        dialog().information(s.arg(Pmax).arg(Isat).arg(alpha));
        dialog().information("Bracketing beta...");
    }

    // Set left and right beta to bracket the solution,
    // thus the optimum will be inbetween: leftBeta < beta < rightBeta
    leftBeta = rightBeta = log(beta);
    int i=0;
    const int maxIter = 10;
    // Move rightBeta farther right
    while (calcPmax(rightBeta) < Pmax && i++<maxIter) {
        reportBracket("Move rightBeta right");
        rightBeta += 2.;
    }
    i = 0;
    while (calcPmax(leftBeta) > Pmax && i++<maxIter) {
        reportBracket("Move leftBeta left");
        leftBeta -= 2.;
    }
    reportBracket("Final bracket");

    // Check bracketing success
    if (i >= maxIter) {
        QString msg("\nPmax = %1\nIsat = %2\nalpha = %3\nLeft beta = %4\nRight beta = %5"
                    "\nLeft Pmax = %6\nRight Pmax = %7");
        msg = msg.arg(Pmax).arg(Isat).arg(alpha).arg(leftBeta).arg(rightBeta).arg(calcPmax(leftBeta)).arg(calcPmax(rightBeta));
        ThrowException("Could not bracket beta").value(msg).context(this);
    }

    // Now minimise the error
    QPair<double, double> minimum = minimise(Error(this), leftBeta, rightBeta, 1e-9, 100, this);
    double logBeta = minimum.first;
    beta = exp(logBeta);
    PmaxResult = calcPmax(logBeta);
    error =  PmaxResult - Pmax;
    log10beta = log10(beta);

    // Report result
    if (report) {
        QString s("Estimated beta = %1 with error = %2");
        dialog().information(s.arg(beta).arg(error));
    }
}

double LightInhibition::calcPmax(double logBeta) const {

    double
        altBeta = exp(logBeta),
        a = alpha*Isat/log(1 + alpha/altBeta),
        b = alpha/(alpha + altBeta),
        c = altBeta/(alpha + altBeta);
    return a*b*pow(c, altBeta/alpha);
}

double LightInhibition::calcError(double altBeta) const {
    return exp(fabs(calcPmax(altBeta) - Pmax));
}

Error::Error(LightInhibition *caller_)
    : caller(caller_) {
}

double Error::operator()(double const &beta) {
    return caller->calcError(beta);
}

}
