#ifndef BASE_VECTOR_OP_H
#define BASE_VECTOR_OP_H
#include <QVector>

class QObject;

namespace vector_op {

typedef QVector<double> Vec;
typedef double Scalar;

// Unary
void add(Vec &v, const Scalar &x);
void add(Vec &v, const Vec &x, QObject *context=0);
void subtract(Vec &v, const Vec &x, QObject *context=0);
void subtract(Vec &v, const Scalar &x);
void multiply(Vec &v, const Vec &x, QObject *context=0);
void multiply(Vec &v, const Scalar &x);

// Binary
void sum(Vec &v, const Vec &x, const Vec &y, QObject *context=0);
void sum(Vec &v, const Vec &x, const Scalar &y);
void difference(Vec &v, const Vec &x, const Vec &y, QObject *context=0);
void difference(Vec &v, const Vec &x, const Scalar &y);
void product(Vec &v, const Vec &x, const Vec &y, QObject *context=0);
void product(Vec &v, const Vec &x, const Scalar &y);

// Summary
double sum(const Vec &x);
double average(const Vec &x);
double weightedAverage(const Vec &x, const Vec &weights, QObject *context=0);
double sumOfProducts(const Vec &x, const Vec &y, QObject *context=0);

} //namespace

#endif
