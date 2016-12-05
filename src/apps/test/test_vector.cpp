#include <memory>
#include <base/assign.h>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port.h>
#include <base/port_transform.h>
#include <base/vector.h>
#include <base/vector_vector.h>
#include "test_vector.h"

using std::unique_ptr;
using namespace base;

void TestVector::testWithSimulation() {
    BoxBuilder builder;
    Box *sim;
    try {
        builder.
            box("Simulation").
                port("steps").equals(4).
                box("Fibonacci").name("fibonacci").
                endbox().
                box("OutputR").
                    box("PageR").
                        box("PlotR").
                            port("ports").equals("(fibonacci[value])").
                        endbox().
                    endbox().
                endbox().
            endbox();
        sim = builder.content();
        sim->run();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }

    Box *fibonacci = Path("fibonacci").resolveOne<Box>(0);
    const Port *port = fibonacci->port("value");
    const Vector *track = port->trackPtr();
    QVERIFY(track);
    QCOMPARE(track->size(), 5);

    delete sim;
}

void TestVector::testWithSimulationIterated() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            port("iterations").equals(3).
            port("iteration").track().
            port("step").track().
            port("steps").equals(4).
            box("Fibonacci").name("fibonacci").
            endbox().
            box("OutputR").
                box("PageR").
                    box("PlotR").
                        port("ports").equals("(sim[iteration] sim[step] fibonacci[value])").
                    endbox().
                endbox().
            endbox().
        endbox();
    Box *sim = builder.content();

    sim->run();

    // replace this test with reading a data frame from the output
    QWARN("Test needs update");
    /*
    Box *fibonacci = Path("fibonacci").resolveOne<Box>(0);
    const Port
        *portIteration = sim->port("iteration"),
        *portStep = sim->port("step"),
        *portValue = fibonacci->port("value");
    const Vector
        *trackIteration = portIteration->trackPtr(),
        *trackStep = portStep->trackPtr(),
        *trackValue = portValue->trackPtr();

    QVERIFY(trackIteration);
    QVERIFY(trackStep);
    QVERIFY(trackValue);

    QCOMPARE(trackIteration->size(), 15);
    QCOMPARE(trackStep->size(), 15);
    QCOMPARE(trackValue->size(), 15);

    QVector<int> vectorIteration, vectorStep, vectorValue;
    assign(IntVector, &vectorIteration, IntVector, trackIteration->ptr(), PortTransform::Identity, 0);
    assign(IntVector, &vectorStep, IntVector, trackStep->ptr(), PortTransform::Identity, 0);
    assign(IntVector, &vectorValue, IntVector, trackValue->ptr(), PortTransform::Identity, 0);

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
    */
    delete sim;
}
