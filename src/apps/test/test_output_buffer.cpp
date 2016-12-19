#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include "test_output_buffer.h"

using namespace base;

void TestOutputBuffer::testOk() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            port("steps").equals(5).
            box("Fibonacci").name("fibo").
            endbox().
            box("OutputBuffer").name("outputBuffer").
                port("ports").equals("(sim[step] fibo[value])").
            endbox().
        endbox();
    Box *sim;
    try {
        sim = builder.content();
        sim->run();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }

    Port *value = sim->findOne<Port>("fibo[value]");
    const Vector* vector = value->trackPtr();
    const QVector<int> *values = reinterpret_cast<const QVector<int> *>(vector->ptr());

    QCOMPARE(vector->size(), 6);
    QCOMPARE(values->size(), 6);
    QVector<int> expected;
    expected << 1 << 1 << 2 << 3 << 5 << 8;
    QCOMPARE(*values, expected);

    delete sim;
}

