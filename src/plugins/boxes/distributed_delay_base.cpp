#include "distributed_delay_base.h"
#include <base/dialog.h>

namespace boxes {

DistributedDelayBase::DistributedDelayBase(QObject *parent_)
    : xSum(0), parent(parent_)
{
//    base::dialog().information("Create DistributedDelayBase");
}

DistributedDelayBase::~DistributedDelayBase() {
//    base::dialog().information("Destruct DistributedDelayBase");
}

void DistributedDelayBase::scale(double factor) {
    if (factor == 0.) {
        x.fill(0.);
        xSum = 0.;
    }
    else if (factor != 1.) {
        int n = x.size();
        double *v = x.data();
        for (int i = 0; i < n; ++i, ++v) *v *= factor;
        xSum *= factor;
    }
}

const QVector<double>& DistributedDelayBase::take(double proportion) {
    if (proportion == 0.) {
        taken.fill(0., x.size());
    }
    if (proportion == 1.) {
        taken = x;
        x.fill(0.);
        xSum = 0.;
    }
    else {
        int n = x.size();
        taken.resize(n);
        double *source = x.data();
        double *destination = taken.data();
        for (int i = 0; i < n; ++i, ++source, ++destination) {
            *destination = *source*proportion;
            *source -= *destination;
        }
        xSum *= (1. - proportion);
    }
    return taken;
}

void DistributedDelayBase::put(const QVector<double> &addition) {
    const double *source = addition.data();
    double *destination = const_cast<double*>(x.data());
    xSum = 0.;
    int n = x.size();
    for (int i = 0; i < n; ++i, ++source, ++destination) {
        xSum = *destination += *source;
    }
}

const double* DistributedDelayBase::data() const {
    return x.data();
}

double DistributedDelayBase::content() const {
    return xSum;
}

} // namespace
