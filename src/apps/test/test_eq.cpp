#include <base/box.h>
#include <base/box_builder.h>
#include <base/port.h>
#include <base/test_num.h>
#include "test_eq.h"

using namespace base;

void TestEq::testGompertz() {
    BoxBuilder builder;
    Box *sim(0);
    double a = 100,
           b = 0.1;
    QVector<double> x, result, expected;
    x << 30 << 60 << 90;
    expected << 0.2762402 << 0.3705556 << 0.3924199;
    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(2).
                box("eq::Gompertz").name("gompertz").
                    port("a").equals(a).
                    port("b").equals(b).
                    port("x").equals(x).
                    port("dx").equals(10).
                    port("yMax").equals(100).
                endbox().
            endbox();

        sim = builder.content();
        sim->run();
    }
    catch(Exception &ex) {
        ThrowException(QString("Unexpected exception:\n") + ex.what());
    }

    result = sim->resolveOne<Port>("gompertz[y]")->value<QVector<double>>();
    QVERIFY(TestNum::eq(result.at(0), expected.at(0)));
    QVERIFY(TestNum::eq(result.at(1), expected.at(1)));
    QVERIFY(TestNum::eq(result.at(2), expected.at(2)));

    delete sim;
}

void TestEq::testZigmoid() {
    BoxBuilder builder;
    Box *sim(0);
    QVector<double> x, result, expected;
    x << 10 << 20 << 60 << 90 << 100;
    double slope = 80./67.;
    expected
        << 0
        << 5*slope
        << 10*slope
        << 2*slope
        << 80;
    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(2).
                box("eq::Zigmoid").name("zigmoid").
                    port("xMin").equals(25).
                    port("xMax").equals(92).
                    port("yMax").equals(80).
                    port("x").equals(x).
                    port("dx").equals(10).
                endbox().
            endbox();

        sim = builder.content();
        sim->run();
    }
    catch(Exception &ex) {
        ThrowException(QString("Unexpected exception:\n") + ex.what());
    }

    result = sim->resolveOne<Port>("zigmoid[y]")->value<QVector<double>>();
    QVERIFY(TestNum::eq(result.at(0), expected.at(0)));
    QVERIFY(TestNum::eq(result.at(1), expected.at(1)));
    QVERIFY(TestNum::eq(result.at(2), expected.at(2)));
    QVERIFY(TestNum::eq(result.at(3), expected.at(3)));
    QVERIFY(TestNum::eq(result.at(4), expected.at(4)));
}
