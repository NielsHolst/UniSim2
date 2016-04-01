#ifndef GENERAL_H
#define GENERAL_H
#include <QDir>
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

bool isAmpersanded(QString s);
QString removeAmpersand(QString s);
bool isApostrophed(QString s);
bool isParenthesized(QString s);
QString deEmbrace(QString s);

QString locateFile(QDir baseDir, QDir specificDir, QString filePath);
QDir locateDir(QDir baseDir, QDir specificDir);
QDir makeDir(QDir baseDir, QDir specificDir);

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
