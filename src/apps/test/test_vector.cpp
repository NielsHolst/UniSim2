#include <memory>
#include <base/assign.h>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port.h>
#include <base/vector.h>
#include <base/vector_vector.h>
#include "test_vector.h"

using std::unique_ptr;
using namespace base;

void TestVector::testWithBox() {
    unique_ptr<Box> fibonacci( MegaFactory::create<Box>("Fibonacci", "fibonacci", 0) );
    const Port *port = fibonacci->port("value");
    const Vector *track = port->trackPtr();
    QString s;
    QCOMPARE(track->size(), 0);

    fibonacci->initializeFamily();
    fibonacci->resetFamily();
    QCOMPARE(track->size(), 1);
    s = track->toString(0);
    QCOMPARE(s, QString("1"));

    fibonacci->updateFamily();
    QCOMPARE(track->size(), 2);
    s = track->toString(1);
    QCOMPARE(s, QString("1"));

    fibonacci->updateFamily();
    fibonacci->updateFamily();
    fibonacci->updateFamily();
    QCOMPARE(track->size(), 5);
    QCOMPARE(track->toString(0), QString("1"));
    QCOMPARE(track->toString(1), QString("1"));
    QCOMPARE(track->toString(2), QString("2"));
    QCOMPARE(track->toString(3), QString("3"));
    QCOMPARE(track->toString(4), QString("5"));
}

void TestVector::testWithSimulation() {
    BoxBuilder builder;
    builder.
        box("Simulation").
            port("steps").equals(4).
            box("Fibonacci").name("fibonacci").
            endbox().
        endbox();
    Box *sim = builder.content();

    Box *fibonacci = Path("fibonacci").resolveOne<Box>();
    const Port *port = fibonacci->port("value");
    const Vector *track = port->trackPtr();
    QCOMPARE(track->size(), 0);

    sim->run();
    QCOMPARE(track->size(), 5);

    delete sim;
}

void TestVector::testWithSimulationIterated() {
    BoxBuilder builder;
    builder.
        box("Simulation").
            port("iterations").equals(3).
            port("steps").equals(4).
            box("Fibonacci").name("fibonacci").
            endbox().
        endbox();
    Box *sim = builder.content();

    Box *fibonacci = Path("fibonacci").resolveOne<Box>();
    const Port
        *portIteration = sim->port("iteration"),
        *portStep = sim->port("step"),
        *portValue = fibonacci->port("value");
    const Vector
        *trackIteration = portIteration->trackPtr(),
        *trackStep = portStep->trackPtr(),
        *trackValue = portValue->trackPtr();

    sim->run();
    QCOMPARE(trackIteration->size(), 15);
    QCOMPARE(trackStep->size(), 15);
    QCOMPARE(trackValue->size(), 15);

    QVector<int> vectorIteration, vectorStep, vectorValue;
    assign(IntVector, &vectorIteration, IntVector, trackIteration->ptr());
    assign(IntVector, &vectorStep, IntVector, trackStep->ptr());
    assign(IntVector, &vectorValue, IntVector, trackValue->ptr());

    VectorVector vectors;
    vectors.append(trackIteration);
    vectors.append(trackStep);
    vectors.append(trackValue);
    QStringList expected;
    expected
        << "0\t1"
        << "1\t1"
        << "2\t2"
        << "3\t3"
        << "4\t5";
    for (int iter = 0; iter < 3; ++iter) {
        for (int step = 0; step < 5; ++step) {
            int row = 5*iter + step;
            QString s = vectors.toString(row);
            QString exp = QString::number(iter) + "\t" + expected.at(step);
            QCOMPARE(s, exp);
        }
    }
    delete sim;
}
