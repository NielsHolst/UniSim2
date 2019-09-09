/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_GENERAL_H
#define BASE_GENERAL_H
#include <QObject>
#include <QString>
#include <QVector>

#define Class(x) setClassName(this, #x)

namespace base {

void setClassName(QObject *object, QString className);
QString className(const QObject *object);
QString classInheritance(const QObject *object);
bool hasClassName(const QObject *object);

QString fullName(const QObject *object);

QStringList split(QString s, QObject *context = 0);
QStringList split(const char *s, QObject *context = 0);

bool isApostrophed(QString s);
bool isParenthesized(QString s);
QString deEmbrace(QString s);
bool isPath(QString s);

double accum(const QVector<double> &x);
void increment(QVector<double> &x, const QVector<double> &toAdd);
void increment(double *x, const double *toAdd, int n);

template<class T> T bounded(T low, T x, T high);

template<class T> T bounded(T low, T x, T high) {
    if (x < low)
        return low;
    else if (x > high)
        return high;
    return x;
}

} //namespace

#endif
