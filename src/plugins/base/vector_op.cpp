#include <QObject>
#include <exception.h>
#include "vector_op.h"

#define CHECK_SIZE(X) \
    if (X.size() != n) ThrowException("Vectors must be of equals size"). \
                       value2(X.size()).value1(n).context(context_)

namespace vector_op {

//
// Unary
//
void add(Vec &v, const Vec &x, QObject *context_) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ += *sender++;
}

void add(Vec &v, const Scalar &x) {
    int i = 0, n = v.size();
    double *receiver = v.data();
    while (i++ < n)
        *receiver++ += x;
}

void subtract(Vec &v, const Vec &x, QObject *context_) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ -= *sender++;
}

void subtract(Vec &v, const Scalar &x) {
    int i = 0, n = v.size();
    double *receiver = v.data();
    while (i++ < n)
        *receiver++ -= x;
}

void multiply(Vec &v, const Vec &x, QObject *context_) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ *= *sender++;
}

void multiply(Vec &v, const Scalar &x) {
    int i = 0, n = v.size();
    double *receiver = v.data();
    while (i++ < n)
        *receiver++ *= x;
}

//
// Binary
//

void sum(Vec &v, const Vec &x, const Vec &y, QObject *context_) {
    int i = 0, n = x.size();
    CHECK_SIZE(y);
    v.resize(n);
    double *receiver = v.data();
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        *receiver++ = *sender1++ + *sender2++;
}

void sum(Vec &v, const Vec &x, const Scalar &y) {
    int i = 0, n = x.size();
    v.resize(n);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ = *sender++ + y;
}

void difference(Vec &v, const Vec &x, const Vec &y, QObject *context_) {
    int i = 0, n = x.size();
    CHECK_SIZE(y);
    v.resize(n);
    double *receiver = v.data();
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        *receiver++ = *sender1++ - *sender2++;
}

void difference(Vec &v, const Vec &x, const Scalar &y) {
    int i = 0, n = x.size();
    v.resize(n);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ = *sender++ - y;
}

void product(Vec &v, const Vec &x, const Vec &y, QObject *context_) {
    int i = 0, n = x.size();
    CHECK_SIZE(y);
    v.resize(n);
    double *receiver = v.data();
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        *receiver++ = *sender1++ * *sender2++;
}

void product(Vec &v, const Vec &x, const Scalar &y) {
    int i = 0, n = x.size();
    v.resize(n);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ = *sender++ * y;
}

//
// Summary
//

double sum(const Vec &x) {
    int i = 0, n = x.size();
    double receiver = 0.;
    const double *sender = x.data();
    while (i++ < n)
        receiver += *sender++;
    return receiver;
}

double average(const Vec &x) {
    int n = x.size();
    return (n==0) ? 0 : sum(x)/n;
}

double weightedAverage(const Vec &x, const Vec &weights, QObject *context_) {
    int i = 0, n = x.size();
    CHECK_SIZE(weights);
    double weightedSum = 0, sumOfWeights = 0;
    const double *sender1 = x.data();
    const double *sender2 = weights.data();
    while (i++ < n) {
        sumOfWeights += *sender2;
        weightedSum += *sender1++ * *sender2++;
    }
    return (sumOfWeights == 0) ? 0. : weightedSum/sumOfWeights;
}

double sumOfProducts(const Vec &x, const Vec &y, QObject *context_) {
    int i = 0, n = x.size();
    CHECK_SIZE(y);
    double receiver = 0;
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        receiver += *sender1++ * *sender2++;
    return receiver;
}

} // namespace
