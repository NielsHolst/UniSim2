#include <exception.h>
#include "vector_op.h"

#define CHECK_SIZE(X) \
    if (X.size() != n) ThrowException("Vectors must be of equals size"). \
                       value(QString::number(X.size()) + "!=" + QString::number(n))

namespace vector_op {

//
// Unary
//
void plus(Vec &v, const Vec &x) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ += *sender++;
}

void plus(Vec &v, const Scalar &x) {
    int i = 0, n = v.size();
    double *receiver = v.data();
    while (i++ < n)
        *receiver++ += x;
}

void minus(Vec &v, const Vec &x) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ -= *sender++;
}

void minus(Vec &v, const Scalar &x) {
    int i = 0, n = v.size();
    double *receiver = v.data();
    while (i++ < n)
        *receiver++ -= x;
}

void multiply(Vec &v, const Vec &x) {
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

void plus(Vec &v, const Vec &x, const Vec &y) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
    CHECK_SIZE(y);
    double *receiver = v.data();
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        *receiver++ = *sender1++ + *sender2++;
}

void plus(Vec &v, const Vec &x, const Scalar &y) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ = *sender++ + y;
}

void minus(Vec &v, const Vec &x, const Vec &y) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
    CHECK_SIZE(y);
    double *receiver = v.data();
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        *receiver++ = *sender1++ - *sender2++;
}

void minus(Vec &v, const Vec &x, const Scalar &y) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ = *sender++ - y;
}

void multiply(Vec &v, const Vec &x, const Vec &y) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
    CHECK_SIZE(y);
    double *receiver = v.data();
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        *receiver++ = *sender1++ * *sender2++;
}

void multiply(Vec &v, const Vec &x, const Scalar &y) {
    int i = 0, n = v.size();
    CHECK_SIZE(x);
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

double sumOfProducts(const Vec &x, const Vec &y) {
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
