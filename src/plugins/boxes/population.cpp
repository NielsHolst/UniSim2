#include <base/exception.h>
#include <base/path.h>
#include <base/publish.h>
#include "population.h"

using namespace base;

namespace boxes {

PUBLISH(Population)

Population::Population(QString name, QObject *parent)
    : Box(name, parent),
      _cohorts(&cohorts),
      _ageIncrements(&ageIncrements)
{
    Input(initial).equals(100);
    Input(ageIncrement);
    Input(bufferSize).equals(100);
    Input(newBorn);
    Input(gains);
    Input(losses);
    Input(newBorn);
    Output(cohorts);
    Output(ageIncrements);
}

void Population::initialize() {
    _gains = resolveMany<Port>(gains);
    _losses = resolveMany<Port>(gains);
    _newBorns = resolveMany<Port>(gains);
}

void Population::reset() {
    _cohorts.resize(bufferSize);
    _ageIncrements.resize(bufferSize);

    _cohorts.push(initial);
    _ageIncrements.push(0);
}

namespace {

    void add(QVector<double> *receiver, const QVector<double> *vec) {
        Q_ASSERT(receiver->size() == vec->size());
        double *p = receiver->data();
        const double *q = vec->data();
        int i = 0, n = vec->size();
        while (i++ < n)
            *p++ += *q++;
    }

    void add(QVector<double> *receiver, QList<const QVector<double>*> add) {
        for (const QVector<double> *vec : add)
            add(receiver, vec);
    }

    void sub(QVector<double> *receiver, const QVector<double> *vec) {
        Q_ASSERT(receiver->size() == vec->size());
        double *p = receiver->data();
        const double *q = vec->data();
        int i = 0, n = vec->size();
        while (i++ < n)
            *p++ -= *q++;
    }

    void sub(QVector<double> *receiver, QList<const QVector<double>*> add) {
        for (const QVector<double> *vec : add)
            sub(receiver, vec);
    }

}

void Population::update() {
    _cohorts.push(newBorn);
    _ageIncrements.push(ageIncrement);
    add(&cohorts, &gains);
    sub(&cohorts, &losses);
}



}
