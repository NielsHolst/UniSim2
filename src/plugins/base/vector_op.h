#ifndef VECTOR_OP_H
#define VECTOR_OP_H
#include <QVector>


namespace vector_op {

typedef QVector<double> Vec;
typedef double Scalar;

// Unary
void plus(Vec &v, const Vec &x);
void plus(Vec &v, const Scalar &x);
void minus(Vec &v, const Vec &x);
void minus(Vec &v, const Scalar &x);
void multiply(Vec &v, const Vec &x);
void multiply(Vec &v, const Scalar &x);

// Binary
void plus(Vec &v, const Vec &x, const Vec &y);
void plus(Vec &v, const Vec &x, const Scalar &y);
void minus(Vec &v, const Vec &x, const Vec &y);
void minus(Vec &v, const Vec &x, const Scalar &y);
void multiply(Vec &v, const Vec &x, const Vec &y);
void multiply(Vec &v, const Vec &x, const Scalar &y);

// Summary
double sum(const Vec &x);

} //namespace

#endif
