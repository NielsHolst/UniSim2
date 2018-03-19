/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef UTILITIES_H
#define UTILITIES_H

#include <limits>
#include <QDir>
#include <QObject>
#include "general.h"

using std::numeric_limits;

namespace vg {

const double RAD = PI/180.;
const double DEGREES = 1./RAD;
const double DEG_TO_RAD = DEGREES;
const double MAX_EXP = -log(numeric_limits<double>::epsilon());
const double MIN_EXP = -log(1. - numeric_limits<double>::epsilon());

class Model;

QFileInfo findNearestFile(QDir home, QString subFolder, QString fileName);

//! @name Mathematics
//@{
template<class T> int sign(T x);
template<class T> T minMax(T low, T x, T high);
double accum(const QVector<double> &x);
void increment(QVector<double> &x, const QVector<double> &toAdd);
void increment(double *x, const double *toAdd, int n);
QVector<double> add(const QVector<double> &x, const QVector<double> &y);
void scale(QVector<double> &x, double factor);

double pow0(double x, double c, QObject *context = 0);
double negExp(double x);
double divBounded(double x, double y, double bound = std::numeric_limits<double>::max());
double GBFuncResp(double demand, double resource);
int toDayOfYear(int day, int month);
double convertTime(double time, char fromUnit, char toUnit, QObject *context = 0);
//@}

//! @name String handling
//@{
QString fullName(const QObject *object);
void splitAtNamespace(QString s, QString *namespacePart, QString *ownNamePart);
//QStringList splitParentChildExpression(QString expression, QObject *context = 0);

//@}

//! @name Testing
//@{
void writeObjectTree(QObject *root, int level = 0);
void writeStandardTestFile(QString filePath);
//@}

// ========================
// Template implementations

template<class T> int sign(T x) {
    return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}

template<class T> T minMax(T low, T x, T high) {
    if (x < low)
        return low;
    else if (x > high)
        return high;
    return x;
}

} //namespace

#endif
