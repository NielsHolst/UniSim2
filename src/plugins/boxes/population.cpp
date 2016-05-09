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
    Input(firstCohortGain);
    Input(cohortsGain);
    Input(cohortsLoss);

    Output(lastCohortSpill);
    Output(cohorts);
    Output(ageIncrements);
}

void Population::reset() {
    _cohorts.resize(bufferSize);
    _ageIncrements.resize(bufferSize);

    _cohorts.push(initial);
    _ageIncrements.push(0);

    if (!cohortsGain.isEmpty() && cohortsGain.size() != bufferSize) {
        QString msg{"cohortsGain must have same vector size (%1) as bufferSize (%2)"};
        ThrowException(msg.arg(cohortsGain.size()).arg(bufferSize));
    }
    if (!cohortsLoss.isEmpty() && cohortsLoss.size() != bufferSize) {
        QString msg{"cohortsLoss must have same vector size (%1) as bufferSize (%2)"};
        ThrowException(msg.arg(cohortsLoss.size()).arg(bufferSize));
    }
}

namespace {

    void add(QVector<double> *receiver, const QVector<double> *vec) {
        if (vec->isEmpty()) return;
        Q_ASSERT(receiver->size() == vec->size());
        double *p = receiver->data();
        const double *q = vec->data();
        int i = 0, n = vec->size();
        while (i++ < n)
            *p++ += *q++;
    }

//    void add(QVector<double> *receiver, QList<const QVector<double>*> add) {
//        for (const QVector<double> *vec : add)
//            add(receiver, vec);
//    }

    void sub(QVector<double> *receiver, const QVector<double> *vec) {
        if (vec->isEmpty()) return;
        Q_ASSERT(receiver->size() == vec->size());
        double *p = receiver->data();
        const double *q = vec->data();
        int i = 0, n = vec->size();
        while (i++ < n)
            *p++ -= *q++;
    }

//    void sub(QVector<double> *receiver, QList<const QVector<double>*> add) {
//        for (const QVector<double> *vec : add)
//            sub(receiver, vec);
//    }

}

void Population::update() {
    add(&cohorts, &cohortsGain);
    sub(&cohorts, &cohortsLoss);
    lastCohortSpill = _cohorts.at(1);
    _cohorts.push(firstCohortGain);
    _ageIncrements.push(ageIncrement);
}



}
